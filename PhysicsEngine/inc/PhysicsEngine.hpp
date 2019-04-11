#pragma once
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>

namespace Tarbora {
    namespace PhysicsEngine {
        bool Init();
        void Close();
        void Update(float deltaTime);

        btRigidBody *AddSphere(float radius, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        // void AddBox(const glm::vec3 &dimensions, ActorPtr actor, float mass, float friction, float density, float restitution);
        void RemoveObject(btCollisionObject *object);

        // void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);
        // void ApplyForce(float radius, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        // void ApplyTorque(ActorId id, float newtons, const glm::vec3 &direction);
        // bool KinematicMove(ActorId id, const glm::mat4 &matrix);

        // void RenderDiagnostics();
    }

    class ActorMotionState : public btMotionState
    {
    public:
        ActorMotionState(glm::mat4 const & transform) :
            m_Transform(transform) {}

        virtual void getWorldTransform(btTransform &transform) const;
        virtual void setWorldTransform(const btTransform &transform);
        void getWorldTransform(glm::mat4 &transform) const;
        void setWorldTransform(const glm::mat4 &transform);

        glm::vec3 getPosition();
        glm::mat3 getRotation();

        glm::mat4 m_Transform;
    };
}
