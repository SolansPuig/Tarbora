#pragma once
#include "../../Framework/CommonHeaders.hpp"
#include "../../Framework/Global.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    struct RayCastResult {
        RayCastResult(ActorId &hit_id, const glm::vec3 &hit_position, const glm::vec3 &hit_normal, float distance) :
            hit_id(hit_id), hit_position(hit_position), hit_normal(hit_normal), distance(distance)
        {}

        ActorId hit_id;
        glm::vec3 hit_position;
        glm::vec3 hit_normal;
        float distance;
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
        static bool init();

        //! Stop the Physics Engine.
        static void close();

        //! Update the Physics Engine.
        static void update(float delta_time);

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
        static btRigidBody* addSphere(ActorId &id, float radius, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation);

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
        static btRigidBody* addCapsule(ActorId &id, float radius, float height, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation);

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
        static btRigidBody* addBox(ActorId &id, const glm::vec3 &dimensions, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation);

        //! Destroy and unregister a rigid body.
        /*!
            \param object A pointer to the object to destroy.
        */
        static void removeObject(btCollisionObject *object);

        // void CreateTrigger(ActorPtr actor, const glm::vec3 &position, const glm::vec3 &dimensions);

        // bool KinematicMove(ActorId id, const glm::mat4 &matrix);

        //! Perform a raycast test.
        /*!
            \param origin The origin point of the ray.
            \param end The maximum end point of the ray.
            \returns See \ref RayCastResult.
        */
        static std::shared_ptr<RayCastResult> rayCast(const glm::vec3 &origin, const glm::vec3 &end);

        // void RenderDiagnostics();

        inline static glm::vec3 toGLM(const btVector3& v)
        {
            return glm::vec3(v.getX(), v.getY(), v.getZ());
        }

        inline static glm::vec4 toGLM(const btVector4& v)
        {
            return glm::vec4(v.getX(), v.getY(), v.getZ(), v.getW());
        }

        inline static glm::quat toGLM(const btQuaternion& q)
        {
            return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
        }

        inline static btVector3 toBullet(const glm::vec3& v)
        {
            return btVector3(v.x, v.y, v.z);
        }

        inline static btVector4 toBullet(const glm::vec4& v)
        {
            return btVector4(v.x, v.y, v.z, v.w);
        }

        inline static btQuaternion toBullet(const glm::quat& q)
        {
            return btQuaternion(q.x, q.y, q.z, q.w);
        }

        inline static btMatrix3x3 toBullet(const glm::mat3& m)
        {
            return btMatrix3x3(m[0][0], m[1][0], m[2][0],
                m[0][1], m[1][1], m[2][1],
                m[0][2], m[1][2], m[2][2]
            );
        }

        inline static btTransform toBullet(const glm::vec3 &position, const glm::quat &rotation)
        {
            return btTransform(toBullet(rotation), toBullet(position));
        }

    private:
        //! This gets called every physics update.
        static void bulletInternalTickCallback(btDynamicsWorld *world, const btScalar delta_time);

        //! Register a rigid body
        static btRigidBody* addShape(ActorId &id, btCollisionShape *shape, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation);

        // Bullet3 variables
        inline static btDynamicsWorld *dynamics_world_;
        inline static btBroadphaseInterface *broadphase_;
        inline static btCollisionDispatcher *dispatcher_;
        inline static btConstraintSolver *solver_;
        inline static btDefaultCollisionConfiguration *collision_configuration_;
        // BulletDebugDrawer *m_DebugDrawer;

        // List of the current collisions
        typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
        inline static std::set<CollisionPair> previous_tick_collisions_;

        // inline static std::list<ActorId>
    };

    //! The position and rotation of an actor.
    class ActorMotionState : public btMotionState
    {
    public:
        ActorMotionState(const glm::vec3 &position, const glm::quat &rotation)
            : position_(position), rotation_(rotation) {}

        virtual void getWorldTransform(btTransform &transform) const;
        virtual void setWorldTransform(const btTransform &transform);
        void getWorldTransform(glm::vec3 &position, glm::quat &rotation) const;
        void setWorldTransform(const glm::vec3 &position, const glm::quat &rotation);

        const glm::vec3& getPosition() { return position_; }
        const glm::quat& getRotation() { return rotation_; }

        glm::vec3 position_;
        glm::quat rotation_;
    };
}
