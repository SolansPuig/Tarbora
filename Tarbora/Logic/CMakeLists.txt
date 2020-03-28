add_subdirectory (PhysicsEngine)

add_library (Logic SHARED
    World.cpp
    ModelComponent.cpp
    TransformComponent.cpp
    PhysicsComponent.cpp
    ControllerComponent.cpp
    AnimationComponent.cpp
    PickupComponent.cpp
)
add_dependencies (Logic Messages)
target_link_libraries (Logic Messages)
target_compile_options(Logic PRIVATE -Werror -Wall -Wextra -Wpedantic)
