#pragma once
#include "../../Global.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    namespace PhysicsEngine {
        bool Init();
        void Close();
        void Update(float deltaTime);

        btRigidBody *AddSphere(unsigned int id, float radius, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        btRigidBody *AddBox(unsigned int id, glm::vec3 &dimensions, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        void RemoveObject(btCollisionObject *object);

        // void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);
        void ApplyForce(btRigidBody *body, float newtons, const glm::vec3 &direction);
        void ApplyTorque(btRigidBody *body, float magnitude, const glm::vec3 &direction);
        void SetVelocity(btRigidBody *body, const glm::vec3 &velocity);
        void Stop(btRigidBody *body);
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
