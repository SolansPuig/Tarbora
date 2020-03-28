#include "PhysicsEngine.hpp"

namespace Tarbora {
    bool PhysicsEngine::init()
    {
        // Configuration of Bullet3
        collision_configuration_ = new btDefaultCollisionConfiguration();
        dispatcher_ = new btCollisionDispatcher(collision_configuration_);
        broadphase_ = new btDbvtBroadphase();
        solver_ = new btSequentialImpulseConstraintSolver();
        dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collision_configuration_);
        // debug_drawer_ = new BulletDebugDrawer();

        if (!collision_configuration_ || !dispatcher_ || !broadphase_ || !solver_ || !dynamics_world_ /*|| !debug_drawer_*/)
        {
            LOG_ERR("PhysicsEngine: Initialization failed.");
            return false;
        }

        // m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
        dynamics_world_->setInternalTickCallback(bulletInternalTickCallback);
        dynamics_world_->setGravity(btVector3(0, -9.81, 0));

        return true;
    }

    void PhysicsEngine::close()
    {
        // Delete all registered objects.
        for (int i = dynamics_world_->getNumCollisionObjects()-1; i >= 0; i--)
        {
            btCollisionObject *obj = dynamics_world_->getCollisionObjectArray()[i];
            removeObject(obj);
        }

        // Delete all Bullet3 variables.
        // delete debug_drawer_;
        delete dynamics_world_;
        delete solver_;
        delete broadphase_;
        delete dispatcher_;
        delete collision_configuration_;
    }

    void PhysicsEngine::update(float delta_time)
    {
        dynamics_world_->stepSimulation(delta_time, 0);
    }

    btRigidBody* PhysicsEngine::addSphere(ActorId &id, float radius, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation)
    {
        btSphereShape *shape = new btSphereShape(radius);
        return addShape(id, shape, mass, friction, restitution, position, rotation);
    }

    btRigidBody* PhysicsEngine::addCapsule(ActorId &id, float radius, float height, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation)
    {
        btCapsuleShape *shape = new btCapsuleShape(radius, height);
        return addShape(id, shape, mass, friction, restitution, position, rotation);
    }

    btRigidBody* PhysicsEngine::addBox(ActorId &id, const glm::vec3 &dimensions, float mass, float friction, float restitution, const glm::vec3 &position, const glm::quat &rotation)
    {
        btBoxShape *shape = new btBoxShape(btVector3(dimensions.x/2, dimensions.y/2, dimensions.z/2));
        return addShape(id, shape, mass, friction, restitution, position, rotation);
    }

    btRigidBody* PhysicsEngine::addShape(ActorId &id, btCollisionShape *shape, float mass, float friction, float restitution,const glm::vec3 &position, const glm::quat &rotation)
    {
        btVector3 local_inertia(0.f, 0.f, 0.f);
        if (mass > 0.f) shape->calculateLocalInertia(mass, local_inertia);

        ActorMotionState *motion_state = new ActorMotionState(position, rotation);
        btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, local_inertia);
        info.m_restitution = restitution;
        info.m_friction = friction;

        btRigidBody *body = new btRigidBody(info);
        body->setUserPointer(&id);
        dynamics_world_->addRigidBody(body);

        return body;
    }

    void PhysicsEngine::removeObject(btCollisionObject *object)
    {
        dynamics_world_->removeCollisionObject(object);

        // Remove all the collision pairs
        for (auto itr = previous_tick_collisions_.begin(); itr != previous_tick_collisions_.end();)
        {
            auto next = itr;
            next++;

            if (itr->first == object || itr->second == object)
            {
                previous_tick_collisions_.erase(itr);
            }

            itr = next;
        }

        if (btRigidBody *body = btRigidBody::upcast(object))
        {
            delete body->getMotionState();
            delete body->getCollisionShape();

            for (int i = body->getNumConstraintRefs()-1; i >= 0; i--)
            {
                btTypedConstraint *constraint = body->getConstraintRef(i);
                dynamics_world_->removeConstraint(constraint);
                delete constraint;
            }
        }

        delete object;
    }

    void PhysicsEngine::addConstraint(btTypedConstraint *constraint)
    {
        dynamics_world_->addConstraint(constraint, true);
    }

    void PhysicsEngine::removeConstraint(btTypedConstraint *constraint)
    {
        dynamics_world_->removeConstraint(constraint);
    }

    std::shared_ptr<RayCastResult> PhysicsEngine::rayCast(const glm::vec3 &origin, const glm::vec3 &end)
    {
        btVector3 ray_origin = btVector3(origin.x, origin.y, origin.z);
        btVector3 ray_end = btVector3(end.x, end.y, end.z);
        btCollisionWorld::ClosestRayResultCallback ray_callback(ray_origin, ray_end);
        dynamics_world_->rayTest(ray_origin, ray_end, ray_callback);

        if (ray_callback.hasHit()) {
            ActorId id = *(ActorId*)(ray_callback.m_collisionObject->getUserPointer());
            btVector3 hit_position = ray_callback.m_hitPointWorld;
            btVector3 hit_normal = ray_callback.m_hitNormalWorld;
            glm::vec3 position = glm::vec3(hit_position.x(), hit_position.y(), hit_position.z());

            std::shared_ptr<RayCastResult> result(new RayCastResult(
                id,
                position,
                glm::vec3(hit_normal.x(), hit_normal.y(), hit_normal.z()),
                glm::distance(origin, position)
            ));

            return result;
        }
        else
        {
            return std::shared_ptr<RayCastResult>();
        }
    }

    void PhysicsEngine::bulletInternalTickCallback(btDynamicsWorld *world, const btScalar delta_time)
    {
        UNUSED(delta_time);
        // Check the collisions and store them in the list.
        std::set<CollisionPair> current_tick_collisions;
        btDispatcher *dispatcher = world->getDispatcher();
        for (int id = 0; id < dispatcher->getNumManifolds(); id++)
        {
            const btPersistentManifold *manifold = dispatcher->getManifoldByIndexInternal(id);
            const btRigidBody *body_a = static_cast<const btRigidBody *>(manifold->getBody0());
            const btRigidBody *body_b = static_cast<const btRigidBody *>(manifold->getBody1());

            const bool swapped = body_a > body_b;
            const btRigidBody *sorted_body_a = swapped ? body_b : body_a;
            const btRigidBody *sorted_body_b = swapped ? body_a : body_b;

            const CollisionPair collision = std::make_pair(sorted_body_a, sorted_body_b);
            current_tick_collisions.insert(collision);

            if (previous_tick_collisions_.find(collision) == previous_tick_collisions_.end())
            {
                // ActorId id_a = *(ActorId*)body_a->getUserPointer();
                // ActorId id_b = *(ActorId*)body_b->getUserPointer();
                // LOG_DEBUG("Collision between %s and %s!", id_a.c_str(), id_b.c_str());
            }
        }

        std::set<CollisionPair> removed_collisions;
        std::set_difference(previous_tick_collisions_.begin(), previous_tick_collisions_.end(),
            current_tick_collisions.begin(), current_tick_collisions.end(),
            std::inserter(removed_collisions, removed_collisions.begin()));

        for (auto itr = removed_collisions.begin(); itr != removed_collisions.end(); itr++)
        {
            // LOG_DEBUG("No more collision!");
        }

        previous_tick_collisions_ = current_tick_collisions;
    }

    void ActorMotionState::getWorldTransform(btTransform &transform) const
    {
        transform = PhysicsEngine::toBullet(position_, rotation_);
    }

    void ActorMotionState::setWorldTransform(const btTransform &transform)
    {
        position_ = PhysicsEngine::toGLM(transform.getOrigin());
        rotation_ = PhysicsEngine::toGLM(transform.getRotation());
    }

    void ActorMotionState::getWorldTransform(glm::vec3 &position, glm::quat &rotation) const
    {
        rotation = rotation_;
        position = position_;
    }

    void ActorMotionState::setWorldTransform(const glm::vec3 &position, const glm::quat &rotation)
    {
        rotation_ = rotation;
        position_ = position;
    }
}
