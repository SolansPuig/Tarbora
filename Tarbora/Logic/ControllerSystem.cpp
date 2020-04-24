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

#include "ControllerSystem.hpp"
#include "PhysicsSystem.hpp"
#include "EntitySystem.hpp"

namespace Tarbora {
  ControllerComponent::ControllerComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    speed = table.get<float>("speed", 5.f, true);
    run_speed = table.get<float>("run_speed", 8.f, true);
    rotation_speed = table.get<float>("rotation_speed", 1.f, true);
    facing_clamp = table.get<glm::vec3>("facing_clamp", {70.f, 70.f, 70.f}, true);

    if (start_enabled_)
      enable();
  }

  ComponentPtr ControllerSystem::controllerFactory(
    const ActorId &id, const LuaTable &table)
  {
    return std::make_shared<ControllerComponent>(id, table);
  }

  SightComponent::SightComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    eye_position = table.get<glm::vec3>("eye_position", glm::vec3(0.f), true);
    look_distance = table.get<float>("look_distance", 10.f, true);

    if (start_enabled_)
      enable();
  }

  ComponentPtr ControllerSystem::sightFactory(const ActorId &id, const LuaTable &table)
  {
    return std::make_shared<SightComponent>(id, table);
  }

  GrabComponent::GrabComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    if (start_enabled_)
      enable();
  }

  ComponentPtr ControllerSystem::grabFactory(const ActorId &id, const LuaTable &table)
  {
    return std::make_shared<GrabComponent>(id, table);
  }

  ControllerSystem::ControllerSystem(World *world) :
    System(world)
  {
    components->registerFactory(
      "controller", FCTBIND(&ControllerSystem::controllerFactory));
    components->registerFactory("sight", FCTBIND(&ControllerSystem::sightFactory));
    components->registerFactory("grab", FCTBIND(&ControllerSystem::grabFactory));
       
    subscribe("set_movement", MSGBIND(&ControllerSystem::setMovement));
    subscribe("set_rotation", MSGBIND(&ControllerSystem::setRotation));
    subscribe("set_facing", MSGBIND(&ControllerSystem::setFacing));
    subscribe("grab", MSGBIND(&ControllerSystem::grabObject));
    subscribe("release", MSGBIND(&ControllerSystem::releaseObject));
    subscribe("grab_distance", MSGBIND(&ControllerSystem::grabDistance));
  }

  void ControllerSystem::setMovement(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    ActorId id = m.getId();

    auto controller = components->getComponent<ControllerComponent>(id);

    if (controller && controller->enabled())
    {
      //  Set movement
      controller->movement = controller->speed * m.getDirection();
      bool walking_old = controller->walking;
      controller->walking = glm::length(controller->movement) != 0.f;

      // Send an event if the state changes
      if (controller->walking != walking_old)
      {
        triggerLocal("move_event", Message::Event(
                       controller->owner,
                       controller->walking ? "walk" : "stop"
                     ));
      }
    }
  }

  void ControllerSystem::setRotation(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    ActorId id = m.getId();

    auto controller = components->getComponent<ControllerComponent>(id);
    if (controller && controller->enabled())
    {
      controller->rotation = controller->rotation_speed * m.getDirection();
    }
  }

  void ControllerSystem::setFacing(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    ActorId id = m.getId();

    auto sight = components->getComponent<SightComponent>(id);
    auto controller = components->getComponent<ControllerComponent>(id);

    if (controller && controller->enabled())
    {
      controller->facing += m.getDirection();
      controller->facing = glm::clamp(
        controller->facing,
        -controller->facing_clamp,
        controller->facing_clamp
      );

      if (sight && sight->enabled())
      {
        glm::vec3 facing = glm::radians(controller->facing);
        sight->look_direction =
          glm::quat(glm::vec3(facing.x, 0.f, -facing.y)) *
          glm::quat(glm::vec3(0.f, 0.f, 1.f));
      }
    }
  }

  void ControllerSystem::grabObject(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m.getId();

    auto sight = components->getComponent<SightComponent>(id);
    auto grab = components->getComponent<GrabComponent>(id);

    if (grab && sight && grab->enabled() && sight->enabled())
    {
      grab->target = sight->target;
      grab->distance = sight->target_distance;

      auto target = components->getComponent<RigidbodyComponent>(grab->target);
      if (target && target->enabled())
      {
        glm::mat4 local = target->getLocalTransform();
        grab->pivot = local * glm::vec4(sight->target_position, 1.f);
      }
    }
  }

  void ControllerSystem::releaseObject(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m.getId();

    auto grab = components->getComponent<GrabComponent>(id);

    if (grab && grab->enabled())
    {
      grab->target = "";
    }
  }

  void ControllerSystem::grabDistance(const MessageSubject &, const MessageBody &body)
  {
    Message::LookAt m(body);
    ActorId id = m.getId();

    auto grab = components->getComponent<GrabComponent>(id);

    if (grab && grab->enabled())
    {
      grab->distance = std::max(1.f, grab->distance + m.getDistance());
    }
  }

  void ControllerSystem::update(float)
  {
    auto comps = components->getComponents<ControllerComponent>();
    for (auto component : comps)
    {
      // Get the necessary components
      auto controller = std::static_pointer_cast<ControllerComponent>(component);
      const ActorId &id = controller->owner;

      auto transform = components->getComponent<TransformComponent>(id);
      auto rb = components->getComponent<RigidbodyComponent>(id);
      auto sight = components->getComponent<SightComponent>(id);
      auto grab = components->getComponent<GrabComponent>(id);


      if (controller->enabled() && rb && rb->enabled() && transform)
      {
        // TODO: This should happen on initalization, not here
        rb->setAngularFactor({0.f, 1.f, 0.f});


        // Check if the entity is on the ground or falling
        auto ray = rb->raycast(
          glm::vec3(0.f),            // From the center
          glm::vec3(0.f, -1.f, 0.f), // Point down
          rb->height/2.f        // To the feet
        );

        if (ray.hit_id != "")
        {
          // Is falling / flying
          if (!controller->on_ground)
          {
            rb->setDamping(0.9999999f, 0.f);
            // TODO: Just hit the ground... Do something!
          }
          controller->on_ground = true;
        }
        else
        {
          // Is on the ground
          if (controller->on_ground)
          {
            rb->setDamping(0.f, 0.f);
            // TODO: Is falling... Do something!
          }
          controller->on_ground = false;
        }

        // Check where the entity is looking at
        if (sight && sight->enabled())
        {
          auto ray = rb->raycast(
            sight->eye_position,
            sight->look_direction * glm::vec3(0.f, 0.f, 1.f),
            sight->look_distance
          );

          ActorId target = "";
          if (ray.hit_id != "")
          {
            // Is looking at something
            target = ray.hit_id;
            sight->target_distance = ray.distance;
            sight->target_position = ray.hit_position;
          }
          if (sight->target != target)
          {
            // If that's new, notify it
            sight->target = target;
            Message::LookAt msg(id, sight->target, sight->target_distance);
            msg.setPosition(sight->target_position);
            trigger("look_at", msg);
          }
        }

        // Move the grabbed object (if any)
        if (grab && grab->enabled())
        {
          if (grab->target != "") // Has a target
          {
            auto target = components->getComponent<RigidbodyComponent>(grab->target);
            if (target && target->enabled())
            {
              bool static_target = target->isStatic();

              // If no constraint is created, create one
              // Static targets don't need a constraint
              if (!grab->constraint && !static_target)
              {
                target->alwaysActive(true);
                grab->constraint = std::make_shared<PointConstraint>(
                  target.get(),
                  grab->pivot
                );
                grab->constraint->setAngularFactor({0.f, 0.f, 0.f});
              }

              // Now move it
              if (sight && sight->enabled())
              {
                glm::mat4 world = rb->getWorldTransform();
                glm::vec3 direction = sight->look_direction * glm::vec3(0.f, 0.f, 1.f);
                glm::vec3 pos = world * glm::vec4(direction * grab->distance, 1.f);

                if (grab->constraint)
                  grab->constraint->setPosB(pos);
                else
                {
                  // Doesn't have constraint, so must be static. Just move it's transform
                  glm::mat4 tworld = target->getWorldTransform();
                  glm::mat4 tlocal = target->getLocalTransform();
                  glm::vec3 lpos = tlocal * glm::vec4(pos, 1.f);
                  glm::vec3 center = tworld * glm::vec4(lpos - grab->pivot, 1.f);

                  if (grab->enable_grid && grab->grid != 0.0f)
                    target->setPosition(glm::round(center/grab->grid) * grab->grid);
                  else
                    target->setPosition(center);
                }
              }
            }
          }
          else if (grab->constraint) // Has a constraint but no target, so destroy it
          {
            grab->constraint.reset();
          }
        }

        // Update the rigidbody component
        rb->applyImpulse(controller->movement);
        rb->setRotation(controller->rotation);

        // Update the animation of the head and neck
        glm::vec3 head(controller->facing.x, 0.f, controller->facing.z);
        glm::vec3 neck(0.f, controller->facing.y, 0.f);

        Message::MoveNode msg1(id, "head");
        msg1.setOrientation(glm::quat(glm::radians(head)));
        trigger("move_node", msg1);

        Message::MoveNode msg2(id, "neck");
        msg2.setOrientation(glm::quat(glm::radians(neck)));
        trigger("move_node", msg2);
      }
    }
  }
}
