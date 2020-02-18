add_subdirectory (PhysicsEngine)

add_library (Logic SHARED
    World.cpp
    ModelComponent.cpp
    TransformComponent.cpp
    PhysicsComponent.cpp
    ControllerComponent.cpp
    AnimationComponent.cpp
)

target_compile_options(Logic PRIVATE -Werror -Wall -Wextra -Wpedantic)
