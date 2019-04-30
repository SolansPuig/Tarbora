#pragma once
#include "../../Global.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    class PhysicsEngine {
    public:
        static bool Init();
        static void Close();
        static void Update(float deltaTime);

        static btRigidBody *AddSphere(unsigned int id, float radius, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        static btRigidBody *AddBox(unsigned int id, glm::vec3 &dimensions, float mass, float friction, float density, float restitution, glm::mat4 &transform);
        static void RemoveObject(btCollisionObject *object);

        // void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);
        static void ApplyForce(btRigidBody *body, float newtons, const glm::vec3 &direction);
        static void ApplyTorque(btRigidBody *body, float magnitude, const glm::vec3 &direction);
        static void SetVelocity(btRigidBody *body, const glm::vec3 &velocity);
        static void Stop(btRigidBody *body);
        // bool KinematicMove(ActorId id, const glm::mat4 &matrix);

        // void RenderDiagnostics();

    private:
        static void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);
        static btRigidBody *AddShape(unsigned int id, btCollisionShape *shape, float mass, float friction, float density, float restitution, glm::mat4 &transform);

        inline static btDynamicsWorld *m_DynamicsWorld;
        inline static btBroadphaseInterface *m_Broadphase;
        inline static btCollisionDispatcher *m_Dispatcher;
        inline static btConstraintSolver *m_Solver;
        inline static btDefaultCollisionConfiguration *m_CollisionConfiguration;
        // BulletDebugDrawer *m_DebugDrawer;

        typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
        typedef std::set<CollisionPair> CollisionPairs;
        inline static CollisionPairs m_PreviousTickCollisionPairs;
    };

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
