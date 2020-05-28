add_subdirectory (GraphicsEngine)

add_library (GraphicViews SHARED
    HumanView.cpp
    Scene.cpp
    SceneNode.cpp
    GameLayer.cpp
    ActorModel.cpp
    AnimationController.cpp
    InventoryLayer.cpp
    Skybox.cpp
)
add_dependencies (GraphicViews Messages)
target_compile_options(GraphicViews PRIVATE -Werror -Wall -Wextra -Wpedantic)
