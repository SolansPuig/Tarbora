add_subdirectory (External)

add_library (PhysicsEngine SHARED
  PhysicsEngine.cpp
  Rigidbody.cpp
  Constraint.cpp
)
target_link_libraries(PhysicsEngine PUBLIC BulletDynamics BulletCollision LinearMath)
target_compile_options(PhysicsEngine PRIVATE -Werror -Wall -Wextra -Wpedantic)
