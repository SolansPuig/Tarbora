include_directories(bullet3/src)
add_subdirectory (bullet3)
add_library (PhysicsEngine SHARED src/PhysicsEngine.cpp src/RigidBody.cpp)
