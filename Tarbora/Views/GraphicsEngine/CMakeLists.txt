add_subdirectory (glfw)
add_library (GraphicsEngine SHARED
    Window.cpp
    GraphicsEngine.cpp
    ImGuiBuild.cpp
    Gui.cpp
    Material.cpp
    Texture.cpp
    TextureInternal.cpp
    Shader.cpp
    Mesh.cpp
    MeshInternal.cpp
    Input.cpp
    RenderQueue.cpp
    Renderer.cpp
)
add_library (imgui SHARED imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui_widgets.cpp)
