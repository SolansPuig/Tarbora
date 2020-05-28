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
  }

  SightComponent::SightComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    eye_position = table.get<glm::vec3>("eye_position", glm::vec3(0.f), true);
    look_distance = table.get<float>("look_distance", 10.f, true);
  }

  GrabComponent::GrabComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {

  }

  ControllerSystem::ControllerSystem(World *world) :
    System(world)
  {
    components->registerFactory("controller", [&](auto id, auto table)
    {
      return std::make_shared<ControllerComponent>(id, table);
    });
    components->registerFactory("sight", [&](auto id, auto table)
    {
      return std::make_shared<SightComponent>(id, table);
    });
    components->onEnable("controller", [&](auto comp)
    {
      auto controller = std::static_pointer_cast<ControllerComponent>(comp);
      if (auto rb = components->require<RigidbodyComponent>(controller->owner))
      {
        rb->setAngularFactor({0.f, 1.f, 0.f});
        return true;
      }
      return false;
    });
    components->onDisable("sight", [&](auto comp)
    {
      auto sight = std::static_pointer_cast<SightComponent>(comp);
      sight->target = "";
      trigger("look_at", Message::LookAt(sight->owner, "", 0.f));
      return true;
    });

    subscribe("set_movement", MSGBIND(&ControllerSystem::setMovement));
    subscribe("set_rotation", MSGBIND(&ControllerSystem::setRotation));
    subscribe("set_facing", MSGBIND(&ControllerSystem::setFacing));
  }

  void ControllerSystem::setMovement(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    ActorId id = m.getId();

    auto controller = components->getComponent<ControllerComponent>(id);

    if (controller && controller->enabled())
    {
      //  Set movement
      if (glm::length(m.getDirection()) != 0.f)
      {
        controller->movement = controller->speed * glm::normalize(m.getDirection());
      }
      else
      {
        controller->movement = glm::vec3(0.f, 0.f, 0.f);
      }

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
        sight->look_direction = glm::quat(glm::vec3(facing.x, 0.f, -facing.y));
      }
    }
  }

  void ControllerSystem::checkGround(
    std::shared_ptr<ControllerComponent> controller,
    std::shared_ptr<RigidbodyComponent> rb
  )
  {
    auto ray = rb->raycast(glm::vec3(0.f), glm::vec3(0.f, -1.f, 0.f), rb->height/2.f);

    if (ray.hit_id != "")
    {
      // Is falling / flying
      if (!controller->on_ground)
      {
        rb->setDamping(0.9999999f, 0.f);
        triggerLocal("falling_event", Message::Event(controller->owner, "hit"));
      }
      controller->on_ground = true;
    }
    else
    {
      // Is on the ground
      if (controller->on_ground)
      {
        rb->setDamping(0.f, 0.f);
        triggerLocal("falling_event", Message::Event(controller->owner, "falling"));
      }
      controller->on_ground = false;
    }
  }

  void ControllerSystem::checkSight(std::shared_ptr<RigidbodyComponent> rb)
  {
    auto sight = components->getComponent<SightComponent>(rb->owner);
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
        Message::LookAt msg(rb->owner, sight->target, sight->target_distance);
        msg.setPosition(sight->target_position);
        trigger("look_at", msg);
      }
    }
  }


  void ControllerSystem::update(float)
  {
    auto comps = components->getComponents<ControllerComponent>();
    for (auto component : comps)
    {
      // Get the necessary components
      auto controller = std::static_pointer_cast<ControllerComponent>(component.lock());
      const ActorId &id = controller->owner;

      auto rb = components->getComponent<RigidbodyComponent>(id);

      if (controller->enabled() && rb && rb->enabled())
      {
        // Check if the entity is on the ground or falling
        checkGround(controller, rb);

        // Check where the entity is looking at
        checkSight(rb);

        // Update the rigidbody component
        rb->applyImpulse(controller->movement);
        rb->setRotation(controller->rotation);

        // Update the animation of the head and neck
        glm::vec3 head(controller->facing.x, 0.f, controller->facing.z);
        glm::vec3 neck(0.f, controller->facing.y, 0.f);
        glm::vec3 arm(-20.f, -controller->facing.x + 70.f, 80.f);

        Message::MoveNode msg1(id, "head");
        msg1.setOrientation(glm::quat(glm::radians(head)));
        trigger("move_node", msg1);

        Message::MoveNode msg2(id, "neck");
        msg2.setOrientation(glm::quat(glm::radians(neck)));
        trigger("move_node", msg2);

        Message::MoveNode msg3(id, "arm_r");
        msg3.setOrientation(glm::quat(glm::radians(arm)));
        trigger("move_node", msg3);
      }
    }
  }

  GrabSystem::GrabSystem(World *w) :
    System(w)
  {
    components->registerFactory("grab", [&](auto id, auto table)
    {
      return std::make_shared<GrabComponent>(id, table);
    });
    components->onDisable("grab", [&](auto comp)
    {
      auto grab =std::static_pointer_cast<GrabComponent>(comp);
      grab->target = "";
      return true;
    });

    subscribe("grab", [&](auto, auto body)
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
          grab->pivot = local * glm::vec4(
            sight->target_position, 1.f
          );
        }
      }
    });

    subscribe("release", [&](auto, auto body)
    {
      Message::Actor m(body);
      ActorId id = m.getId();

      auto grab = components->getComponent<GrabComponent>(id);

      if (grab && grab->enabled())
      {
        grab->target = "";
      }
    });

    subscribe("grab_distance", [&](auto, auto body)
    {
      Message::LookAt m(body);
      ActorId id = m.getId();

      auto grab = components->getComponent<GrabComponent>(id);

      if (grab && grab->enabled())
      {
        grab->distance = std::max(1.f, grab->distance + m.getDistance());
      }
    });

    subscribe("enable_grid", [&](auto, auto body)
    {
      Message::Actor m(body);
      auto grab = components->getComponent<GrabComponent>(m->id());
      if (grab && grab->enabled())
      {
        grab->enable_grid = true;
      }
    });

    subscribe("disable_grid", [&](auto, auto body)
    {
      Message::Actor m(body);
      auto grab = components->getComponent<GrabComponent>(m->id());
      if (grab && grab->enabled())
      {
        grab->enable_grid = false;
      }
    });

    subscribe("grid_size", [&](auto, auto body)
    {
      Message::Actor m(body);
      auto grab = components->getComponent<GrabComponent>(m->id());
      if (grab && grab->enabled())
      {
        grab->grid = 0.5; // Temporary
      }
    });
  }

  void GrabSystem::update(float)
  {
    auto comps = components->getComponents<GrabComponent>();
    for (auto component : comps)
    {
      // Get the necessary components
      auto grab = std::static_pointer_cast<GrabComponent>(component.lock());
      const ActorId &id = grab->owner;

      if (grab->target != "" && grab->enabled())
      {
        auto trb = components->getComponent<RigidbodyComponent>(grab->target);
        if (trb && trb->enabled())
        {
          bool is_static = trb->isStatic();

          // Create a constraint if it's not created yet. Skip that if target is static
          if (!grab->constraint && !is_static)
          {
            trb->alwaysActive(true);
            trb->setVelocity({0.f, 0.f, 0.f});
            trb->setRotation({0.f, 0.f, 0.f});
            grab->constraint = std::make_shared<PointConstraint>(trb.get(), grab->pivot);
            grab->constraint->setAngularFactor({0.f, 0.f, 0.f});
          }

          // Move the target
          auto sight = components->getComponent<SightComponent>(id);
          auto crb = components->getComponent<RigidbodyComponent>(id);
          if (sight && sight->enabled() && crb && crb->enabled())
          {
            // Calculate the wanted position
            glm::mat4 cworld = crb->getWorldTransform();
            glm::vec3 dir = sight->look_direction * glm::vec3(0.f, 0.f, 1.f);
            glm::vec3 pos = cworld *
              glm::vec4(sight->eye_position + dir * grab->distance, 1.f);

            if(!is_static)
            {
              // If it's not static just move the constraint
              grab->constraint->setPosB(pos);
            }
            else
            {
              // If it's static, it must be moved manually, so we need to find the center
              glm::mat4 tworld = trb->getWorldTransform();
              glm::vec3 lpos = trb->getLocalTransform() * glm::vec4(pos, 1.f);
              glm::vec3 center = tworld * glm::vec4(lpos - grab->pivot, 1.f);
             
              // If there's a grid, the new center must fit the grid
              if (grab->enable_grid && grab->grid != 0.f)
              {
                center = glm::round(center/grab->grid) * grab->grid;
              }

              // Finally move it
              trb->setPosition(center);
            }
          }
        }
      }
      else if (grab->constraint)
      {
        // It has a constraint but not a target, so destroy the constraint
        grab->constraint.reset();
      }
    }
  }
}
