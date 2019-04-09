#include "PhysicsEngine.hpp"

namespace Tarbora {
    namespace PhysicsEngine {
        btDynamicsWorld *m_DynamicsWorld;
        btBroadphaseInterface *m_Broadphase;
        btCollisionDispatcher *m_Dispatcher;
        btConstraintSolver *m_Solver;
        btDefaultCollisionConfiguration *m_CollisionConfiguration;
        BulletDebugDrawer *m_DebugDrawer;

        typedef std::map<btRigidBody const *, ActorId>RigidBodyToActorId;
        typedef std::map<ActorId, btRigidBody const *>ActorIdToRigidBody;
        RigidBodyToActorId m_ActorIdToRigidBody;
        ActorIdToRigidBody m_RigidBodyToActorId;
        ActorId FindActorId(btRigidBody const * rb) const { return m_RigidBodyToActorId[rb]; }
        btRigidBody *FindRigidBody(ActorId id) const { return m_ActorIdToRigidBody[id]; }

        typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
        typedef std::set<CollisionPair> CollisionPairs;
        CollisionPairs m_PreviousTickCollisionPairs;

        void SendCollisionPairEvent(btPersistentManifold const *manifold, btRigidBody const *body0, btRigidBody const *body1);

        void AddShape(ActorPtr actor, btCollisionShape *shape, float mass, float friction, float density, float restitution);
        void RemoveCollisionObject(btCollisionObject *object);

        static void BulletInternalTickCallback(btDynamicsWorld * const world, btScalar const timeStep);


        bool Init()
        {
            m_CollisionConfiguration = new btDefaultCollisionConfiguration();
            m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration.get());
            m_Broadphase = new btObvBroadphase();
            m_Solver = new btSequentialImpulseConstraintSolver();
            m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
            m_DebugDrawer = new BulletDebugDrawer();

            if (!m_CollisionConfiguration || !m_Dispatcher || !m_Broadphase || !m_Solver || !m_DynamicsWorld || !m_DebugDrawer)
            {
                LOG_ERR("PhysicsEngine: Initialization failed.");
                return false;
            }

            m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
            m_DynamicsWorld->setInternalTickCallback(BulletInternalTickCallback);
            m_DynamicsWorld->setWorldUserInfo(this);

            return true;
        }

        void Shutdown()
        {
            for (int i = m_DynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--)
            {
                btCollisionObject * const obj = m_DynamicsWorld->getCollisionObjectArray()[i];
                RemoveCollisionObject(obj);
            }

            m_ActorBodies.clear();

            delete m_DebugDrawer;
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

    }
}
