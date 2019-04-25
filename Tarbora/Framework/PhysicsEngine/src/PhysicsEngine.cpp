#include "../inc/PhysicsEngine.hpp"
#include <set>
#include <algorithm>

namespace Tarbora {
    namespace PhysicsEngine {
        btDynamicsWorld *m_DynamicsWorld;
        btBroadphaseInterface *m_Broadphase;
        btCollisionDispatcher *m_Dispatcher;
        btConstraintSolver *m_Solver;
        btDefaultCollisionConfiguration *m_CollisionConfiguration;
        // BulletDebugDrawer *m_DebugDrawer;

        typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
        typedef std::set<CollisionPair> CollisionPairs;
        CollisionPairs m_PreviousTickCollisionPairs;

        btRigidBody *AddShape(unsigned int id, btCollisionShape *shape, float mass, float friction, float density, float restitution, glm::mat4 &transform);

        void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);


        bool Init()
        {
            m_CollisionConfiguration = new btDefaultCollisionConfiguration();
            m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
            m_Broadphase = new btDbvtBroadphase();
            m_Solver = new btSequentialImpulseConstraintSolver();
            m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
            // m_DebugDrawer = new BulletDebugDrawer();

            if (!m_CollisionConfiguration || !m_Dispatcher || !m_Broadphase || !m_Solver || !m_DynamicsWorld /*|| !m_DebugDrawer*/)
            {
                LOG_ERR("PhysicsEngine: Initialization failed.");
                return false;
            }

            // m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
            m_DynamicsWorld->setInternalTickCallback(BulletInternalTickCallback);
            m_DynamicsWorld->setGravity(btVector3(0, -9.81, 0));

            return true;
        }

        void Close()
        {
            for (int i = m_DynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--)
            {
                btCollisionObject * const obj = m_DynamicsWorld->getCollisionObjectArray()[i];
                RemoveObject(obj);
            }

            // delete m_DebugDrawer;
            delete m_DynamicsWorld;
            delete m_Solver;
            delete m_Broadphase;
            delete m_Dispatcher;
            delete m_CollisionConfiguration;
        }

        void Update(float deltaTime)
        {
            m_DynamicsWorld->stepSimulation(deltaTime, 4);
        }

        btRigidBody *AddSphere(unsigned int id, float radius, float mass, float friction, float density, float restitution, glm::mat4 &transform)
        {
            btSphereShape *const shape = new btSphereShape(radius);
            return AddShape(id, shape, mass, friction, density, restitution, transform);
        }

        btRigidBody *AddBox(unsigned int id, glm::vec3 &dimensions, float mass, float friction, float density, float restitution, glm::mat4 &transform)
        {
            btBoxShape *const shape = new btBoxShape(btVector3(dimensions.x/2, dimensions.y/2, dimensions.z/2));
            return AddShape(id, shape, mass, friction, density, restitution, transform);
        }

        btRigidBody *AddShape(unsigned int id, btCollisionShape *shape, float mass, float friction, float density, float restitution, glm::mat4 &transform)
        {
            btVector3 localInertia(0.f, 0.f, 0.f);
            if (mass > 0.f) shape->calculateLocalInertia(mass, localInertia);

            ActorMotionState *motionState = new ActorMotionState(transform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
            rbInfo.m_restitution = restitution;
            rbInfo.m_friction = friction;

            btRigidBody *const body = new btRigidBody(rbInfo);
            body->setUserIndex(id);
            m_DynamicsWorld->addRigidBody(body);

            return body;
        }

        void RemoveObject(btCollisionObject *object)
        {
            m_DynamicsWorld->removeCollisionObject(object);

            // Remove all the collision pairs
            for (auto itr = m_PreviousTickCollisionPairs.begin(); itr != m_PreviousTickCollisionPairs.end();)
            {
                auto next = itr;
                next++;

                if (itr->first == object || itr->second == object)
                {
                    m_PreviousTickCollisionPairs.erase(itr);
                }

                itr = next;
            }

            if (btRigidBody *const body = btRigidBody::upcast(object))
            {
                delete body->getMotionState();
                delete body->getCollisionShape();
                delete body->getUserPointer();

                for (int i = body->getNumConstraintRefs()-1; i >= 0; i--)
                {
                    btTypedConstraint *const constraint = body->getConstraintRef(i);
                    m_DynamicsWorld->removeConstraint(constraint);
                    delete constraint;
                }
            }

            delete object;
        }

        void ApplyForce(btRigidBody *body, float newtons, const glm::vec3 &direction)
        {
            btVector3 const force(direction.x * newtons, direction.y * newtons, direction.z * newtons);
            body->applyCentralImpulse(force);
            body->activate();
        }

        void ApplyTorque(btRigidBody *body, float magnitude, const glm::vec3 &direction)
        {
            btVector3 const torque(direction.x * magnitude, direction.y * magnitude, direction.z * magnitude);
            body->applyTorqueImpulse(torque);
            body->activate();
        }

        void SetVelocity(btRigidBody *body, const glm::vec3 &velocity)
        {
            btVector3 const vel(velocity.x, velocity.y, velocity.z);
            body->setLinearVelocity(vel);
            body->activate();
        }

        void Stop(btRigidBody *body)
        {
            SetVelocity(body, glm::vec3(0.0f, 0.0f, 0.0f));
        }

        void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep)
        {
            CollisionPairs currentTickPairs;
            btDispatcher *const dispatcher = world->getDispatcher();
            for (int id = 0; id < dispatcher->getNumManifolds(); id++)
            {
                btPersistentManifold const *const manifold = dispatcher->getManifoldByIndexInternal(id);
                btRigidBody const *const body0 = static_cast<btRigidBody const *>(manifold->getBody0());
                btRigidBody const *const body1 = static_cast<btRigidBody const *>(manifold->getBody1());

                const bool swapped = body0 > body1;
                btRigidBody const *const sortedBodyA = swapped ? body1 : body0;
                btRigidBody const *const sortedBodyB = swapped ? body0 : body1;

                CollisionPair const thisPair = std::make_pair(sortedBodyA, sortedBodyB);
                currentTickPairs.insert(thisPair);

                if (m_PreviousTickCollisionPairs.find(thisPair) == m_PreviousTickCollisionPairs.end())
                {
                    LOG_DEBUG("Collision between %u and %u!", body0->getUserIndex(), body1->getUserIndex());
                    // CollisionEvent ev(manifold, body0, body1);
                    // EventManager::Trigger(EventType::Collision, &ev);
                }
            }

            CollisionPairs removedCollisionPairs;
            std::set_difference(m_PreviousTickCollisionPairs.begin(), m_PreviousTickCollisionPairs.end(),
                currentTickPairs.begin(), currentTickPairs.end(),
                std::inserter(removedCollisionPairs, removedCollisionPairs.begin()));

            for (auto itr = removedCollisionPairs.begin(), end = removedCollisionPairs.end(); itr != end; itr++)
            {
                LOG_DEBUG("No more collision!");
            }

            m_PreviousTickCollisionPairs = currentTickPairs;
        }
    }

    void ActorMotionState::getWorldTransform(btTransform &transform) const
    {
        btMatrix3x3 rotation;
        btVector3 position;
        for (int i = 0; i < 3; i++)
        {
            position[i] = m_Transform[3][i];
            for (int j = 0; j < 3; j++)
            {
                rotation[i][j] = m_Transform[j][i];
            }
        }

        transform = btTransform(rotation, position);
    }

    void ActorMotionState::setWorldTransform(const btTransform &transform)
    {
        btMatrix3x3 const &rotation = transform.getBasis();
        btVector3 const &position = transform.getOrigin();
        for (int i = 0; i < 4; i++)
        {
            m_Transform[3][i] = position[i];
            for (int j = 0; j < 4; j++)
            {
                m_Transform[i][j] = rotation[j][i];
            }
        }
    }

    void ActorMotionState::getWorldTransform(glm::mat4 &transform) const
    {
        transform = m_Transform;
    }

    void ActorMotionState::setWorldTransform(const glm::mat4 &transform)
    {
        m_Transform = transform;
    }

    glm::vec3 ActorMotionState::getPosition()
    {
        return glm::vec3(m_Transform[0][3], m_Transform[1][3], m_Transform[2][3]);
    }

    glm::mat3 ActorMotionState::getRotation()
    {
        return glm::mat3(m_Transform);
    }
}
