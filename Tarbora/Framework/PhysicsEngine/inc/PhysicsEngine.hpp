#pragma once
#include "../../CommonHeaders.hpp"
#include "../../Global.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    struct RayCastResult {
        RayCastResult(const ActorId &hit_id, glm::vec3 hit_position, glm::vec3 hit_normal, float distance) :
            m_HitId(hit_id), m_HitPosition(hit_position), m_HitNormal(hit_normal), m_Distance(distance)
        {}

        ActorId m_HitId;
        glm::vec3 m_HitPosition;
        glm::vec3 m_HitNormal;
        float m_Distance;
    };

    //! The system that manages the movement and the collisions of all the actors.
    /*!
        \see RigidBody
        \see ActorMotionState
    */
    class PhysicsEngine {
    public:
        //! Start the Physics Engine.
        /*!
            It must be called on startup.
        */
        static bool Init();

        //! Stop the Physics Engine.
        static void Close();

        //! Update the Physics Engine.
        static void Update(float deltaTime);

        //! Register an sphere shaped rigid body.
        /*!
            \param id The id of the actor that owns that rigid body.
            \param radius The radius of the sphere.
            \param mass The mass ("weight") of the sphere.
            \param friction See \ref RigidBody SetFriction
            \param restitution See \ref RigidBody SetRestitution
            \param transform The transform matrix of the actor.
            \returns A Bullet3's rigid body.
        */
        static btRigidBody *AddSphere(ActorId &id, float radius, float mass, float friction, float restitution, glm::mat4 &transform);

        //! Register an capsule shaped rigid body.
        /*!
            \param id The id of the actor that owns that rigid body.
            \param radius The radius of the capsule.
            \param height The height of the capsule.
            \param mass The mass ("weight") of the capsule.
            \param friction See \ref RigidBody SetFriction
            \param restitution See \ref RigidBody SetRestitution
            \param transform The transform matrix of the actor.
            \returns A Bullet3's rigid body.
        */
        static btRigidBody *AddCapsule(ActorId &id, float radius, float height, float mass, float friction, float restitution, glm::mat4 &transform);

        //! Register a cube shaped rigid body.
        /*!
            \param id The id of the actor that owns that rigid body.
            \param dimensions A vector representing the dimensions of that box, used to calculate the volume and the mass.
            \param mass The mass ("weight") of the box.
            \param friction See \ref RigidBody SetFriction
            \param restitution See \ref RigidBody SetRestitution
            \param transform The transform matrix of the actor.
            \returns A Bullet3's rigid body.
        */
        static btRigidBody *AddBox(ActorId &id, glm::vec3 &dimensions, float mass, float friction, float restitution, glm::mat4 &transform);

        //! Destroy and unregister a rigid body.
        /*!
            \param object A pointer to the object to destroy.
        */
        static void RemoveObject(btCollisionObject *object);

        // void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);

        // bool KinematicMove(ActorId id, const glm::mat4 &matrix);

        //! Perform a raycast test.
        /*!
            \param origin The origin point of the ray.
            \param end The maximum end point of the ray.
            \returns See \ref RayCastResult.
        */
        static std::shared_ptr<RayCastResult> RayCast(glm::vec3 &origin, glm::vec3 &end);

        // void RenderDiagnostics();

    private:
        //! This gets called every physics update.
        static void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);

        //! Register a rigid body
        static btRigidBody *AddShape(ActorId &id, btCollisionShape *shape, float mass, float friction, float restitution, glm::mat4 &transform);

        // Bullet3 variables
        inline static btDynamicsWorld *m_DynamicsWorld;
        inline static btBroadphaseInterface *m_Broadphase;
        inline static btCollisionDispatcher *m_Dispatcher;
        inline static btConstraintSolver *m_Solver;
        inline static btDefaultCollisionConfiguration *m_CollisionConfiguration;
        // BulletDebugDrawer *m_DebugDrawer;

        // List of the current collisions
        typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
        typedef std::set<CollisionPair> CollisionPairs;
        inline static CollisionPairs m_PreviousTickCollisionPairs;
    };

    //! The position and rotation of an actor.
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
