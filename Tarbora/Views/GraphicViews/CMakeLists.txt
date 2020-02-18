add_subdirectory (GraphicsEngine)

add_library (GraphicViews SHARED
    HumanView.cpp
    Scene.cpp
    SceneNode.cpp
    GameLayer.cpp
    ActorModel.cpp
    AnimationController.cpp
    ModelEditor.cpp
)
target_compile_options(GraphicViews PRIVATE -Werror -Wall -Wextra -Wpedantic)
