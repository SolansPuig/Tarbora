add_subdirectory (External/glfw)
add_library (imgui SHARED External/imgui/imgui.cpp External/imgui/imgui_draw.cpp External/imgui/imgui_demo.cpp External/imgui/imgui_widgets.cpp)
target_compile_options(imgui PRIVATE -w)
add_library (imguizmo SHARED External/ImGuizmo/ImGuizmo.cpp)
target_compile_options(imguizmo PRIVATE -w)
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
target_link_libraries(GraphicsEngine PUBLIC imgui imguizmo glfw)
target_compile_options(GraphicsEngine PRIVATE -Werror -Wall -Wextra -Wpedantic)
