/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#include "PhysicsSystem.hpp"
#include "EntitySystem.hpp"

namespace Tarbora {
  PhysicsComponent::PhysicsComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {

  }

  ComponentPtr PhysicsSystem::physicsFactory(const ActorId &id, const LuaTable &table)
  {
    auto component = std::make_shared<PhysicsComponent>(id, table);

    std::string s = table.get<std::string>("shape", "null");

    float friction = table.get<float>("friction", 0.f, true);
    float density = table.get<float>("density", 1.f, true);
    float restitution = table.get<float>("restitution", 0.f, true);

    auto transform = components->getComponent<TransformComponent>(id);


    Shape shape;

    if (s == "sphere")
    {
      float radius = table.get<float>("radius");
      shape = PhysicsEngine::genSphere(radius);
      component->height = 2 * radius;
    }
    else if (s == "capsule")
    {
      float radius = table.get<float>("radius");
      float height = table.get<float>("height");
      shape = PhysicsEngine::genCapsule(radius, height);
      component->height = 2 * radius + height;
    }
    else if (s == "box")
    {
      glm::vec3 size = table.get<glm::vec3>("size");
      shape = PhysicsEngine::genBox(size);
      component->height = size.y;
    }
    else
    {
      LOG_ERR(
        "PhysicsComponent: \"%s\" is not a valid character controller shape.",
        s.c_str());
      return ComponentPtr();
    }

    component->body.init(
      id, shape, friction, density, restitution,
      transform->position,
      transform->orientation
    );

    component->enable();
    return component;
  }


  PhysicsSystem::PhysicsSystem(World *world) :
    System(world)
  {
    components->registerFactory("physics", FCTBIND(&PhysicsSystem::physicsFactory));

    PhysicsEngine::init();

    subscribe("set_position", MSGBIND(&PhysicsSystem::setPosition));
    subscribe("set_orientation", MSGBIND(&PhysicsSystem::setOrientation));
    subscribe("move", MSGBIND(&PhysicsSystem::move));
    subscribe("rotate", MSGBIND(&PhysicsSystem::rotate));
    subscribe("apply_force", MSGBIND(&PhysicsSystem::applyForce));
    subscribe("apply_torque", MSGBIND(&PhysicsSystem::applyTorque));
    subscribe("set_velocity", MSGBIND(&PhysicsSystem::setVelocity));
    subscribe("set_angular_vel", MSGBIND(&PhysicsSystem::setAngularVel));
    subscribe("stop", MSGBIND(&PhysicsSystem::stop));
  }

  void PhysicsSystem::setPosition(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.setPosition(m.getDirection());
  }

  void PhysicsSystem::setOrientation(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.setOrientation(glm::quat(glm::radians(m.getDirection())));
  }

  void PhysicsSystem::move(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
    {
      auto position = physics->body.getPosition() + m.getDirection();
      physics->body.setPosition(position);
    }
  }

  void PhysicsSystem::rotate(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
    {
      auto old = glm::quat(glm::radians(m.getDirection()));
      auto orientation = physics->body.getOrientation() + old;
      physics->body.setOrientation(orientation);
    }
  }

  void PhysicsSystem::applyForce(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.applyForce(m.getDirection());
  }

  void PhysicsSystem::applyTorque(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.applyTorque(m.getDirection());
  }

  void PhysicsSystem::setVelocity(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.setVelocity(m.getDirection());
  }

  void PhysicsSystem::setAngularVel(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.setRotation(m.getDirection());
  }

  void PhysicsSystem::stop(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto physics = components->getComponent<PhysicsComponent>(m.getId());
    if (physics && physics->enabled())
      physics->body.setVelocity({0.f, 0.f, 0.f});
  }

  void PhysicsSystem::update(float delta_time)
  {
    // Before updating the physics engine

    // Update the physics engine
    PhysicsEngine::update(delta_time);

    // After updating the physics engine
    auto comps = components->getComponents<PhysicsComponent>();
    for (auto component : comps)
    {
      auto physics = std::static_pointer_cast<PhysicsComponent>(component);
      if (physics->enabled())
      {
        ActorId &id = physics->owner;

        glm::vec3 position = physics->body.getPosition();
        glm::quat orientation = physics->body.getOrientation();

        //  Update the transform
        auto transform = components->getComponent<TransformComponent>(id);
        if (transform->position != position)
        {
          transform->position = position;
          transform->update = true;
        }
        if (transform->orientation != orientation)
        {
          transform->orientation = orientation;
          transform->update = true;
        }
      }
    }
  }
}
