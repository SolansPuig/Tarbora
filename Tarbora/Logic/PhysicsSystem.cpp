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
  RigidbodyComponent::RigidbodyComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table), Rigidbody(id)
  {
    std::string s = table.get<std::string>("shape", "null");

    friction = table.get<float>("friction", 0.f, true);
    density = table.get<float>("density", 1.f, true);
    restitution = table.get<float>("restitution", 0.f, true);

    if (s == "sphere")
    {
      float radius = table.get<float>("radius");
      shape = PhysicsEngine::genSphere(radius);
      height = 2 * radius;
    }
    else if (s == "capsule")
    {
      float radius = table.get<float>("radius");
      float h = table.get<float>("height");
      shape = PhysicsEngine::genCapsule(radius, h);
      height = 2 * radius + h;
    }
    else if (s == "box")
    {
      glm::vec3 size = table.get<glm::vec3>("size");
      shape = PhysicsEngine::genBox(size);
      height = size.y;
    }
    else
    {
      LOG_ERR(
        "RigidbodyComponent: \"%s\" is not a valid character controller shape.",
        s.c_str());
      error_ = true;
    }
  }

  PhysicsSystem::PhysicsSystem(World *world) :
    System(world)
  {
    components->registerFactory("rigidbody", [&](auto id, auto table)
    {
      return std::make_shared<RigidbodyComponent>(id, table);
    });
    components->onEnable("rigidbody", [&](auto comp)
    {
      auto rb = std::static_pointer_cast<RigidbodyComponent>(comp);
      auto transform = components->getComponent<TransformComponent>(rb->owner);

      if (transform)
      {
        rb->create(transform->position, transform->orientation);
        return true;
      }
      return false;
    });
    components->onDisable("rigidbody", [&](auto comp)
    {
      auto rb = std::static_pointer_cast<RigidbodyComponent>(comp);
      rb->destroy();
      return true;
    });

    PhysicsEngine::init();

    subscribe("set_position", MSGBIND(&PhysicsSystem::setPosition));
    subscribe("set_orientation", MSGBIND(&PhysicsSystem::setOrientation));
    subscribe("move", MSGBIND(&PhysicsSystem::move));
    subscribe("rotate", MSGBIND(&PhysicsSystem::rotate));
    subscribe("apply_force", MSGBIND(&PhysicsSystem::applyForce));
    subscribe("apply_impulse", MSGBIND(&PhysicsSystem::applyImpulse));
    subscribe("apply_torque", MSGBIND(&PhysicsSystem::applyTorque));
    subscribe("set_velocity", MSGBIND(&PhysicsSystem::setVelocity));
    subscribe("set_angular_vel", MSGBIND(&PhysicsSystem::setAngularVel));
    subscribe("stop", MSGBIND(&PhysicsSystem::stop));
  }

  void PhysicsSystem::setPosition(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->setPosition(m.getDirection());
  }

  void PhysicsSystem::setOrientation(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->setOrientation(glm::quat(glm::radians(m.getDirection())));
  }

  void PhysicsSystem::move(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
    {
      auto position = rb->getPosition() + m.getDirection();
      rb->setPosition(position);
    }
  }

  void PhysicsSystem::rotate(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
    {
      auto old = glm::quat(glm::radians(m.getDirection()));
      auto orientation = rb->getOrientation() + old;
      rb->setOrientation(orientation);
    }
  }

  void PhysicsSystem::applyForce(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->applyForce(m.getDirection());
  }

  void PhysicsSystem::applyImpulse(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->applyImpulse(m.getDirection());
  }

  void PhysicsSystem::applyTorque(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->applyTorque(m.getDirection());
  }

  void PhysicsSystem::setVelocity(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->setVelocity(m.getDirection());
  }

  void PhysicsSystem::setAngularVel(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->setRotation(m.getDirection());
  }

  void PhysicsSystem::stop(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto rb = components->getComponent<RigidbodyComponent>(m.getId());
    if (rb && rb->enabled())
      rb->setVelocity({0.f, 0.f, 0.f});
  }

  void PhysicsSystem::update(float delta_time)
  {
    // Before updating the physics engine

    // Update the physics engine
    PhysicsEngine::update(delta_time);

    // After updating the physics engine
    auto comps = components->getComponents<RigidbodyComponent>();
    for (auto component : comps)
    {
      auto rb = std::static_pointer_cast<RigidbodyComponent>(component.lock());
      if (rb && rb->enabled())
      {
        ActorId &id = rb->owner;

        glm::vec3 position = rb->getPosition();
        glm::quat orientation = rb->getOrientation();

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
