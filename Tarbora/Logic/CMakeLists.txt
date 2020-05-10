add_subdirectory (PhysicsEngine)

add_library (Logic SHARED
    System.cpp
    World.cpp
    AnimationSystem.cpp
    Component.cpp
    ControllerSystem.cpp
    EntitySystem.cpp
    PhysicsSystem.cpp
    RenderSystem.cpp
    InventorySystem.cpp
)
add_dependencies (Logic Messages)
target_link_libraries (Logic Messages)
target_compile_options(Logic PRIVATE -Werror -Wall -Wextra -Wpedantic)
