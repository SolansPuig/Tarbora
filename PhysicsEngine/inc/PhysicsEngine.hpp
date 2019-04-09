#pragma once
#include "btBulletDynamicsCommon.h"
#include "Actors.hpp"

namespace Tarbora {
    namespace PhysicsEngine {
        bool Init();
        void Shutdown();
        void SyncVisibleScene();
        void Update();

        void AddSphere(float radius, ActorPtr actor, float mass, float friction, float density, float restitution);
        void AddBox(const glm::vec3 &dimensions, ActorPtr actor, float mass, float friction, float density, float restitution);
        void RemoveActor(ActorId id);

        void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);
        void ApplyForce(ActorId id, float newtons, const glm::vec3 &direction);
        void ApplyTorque(ActorId id, float newtons, const glm::vec3 &direction);
        bool KinematicMove(ActorId id, const glm::mat4 &matrix);

        void RenderDiagnostics();
    }
}
