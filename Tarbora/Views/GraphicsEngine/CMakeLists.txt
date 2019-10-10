add_subdirectory (glfw)
add_library (GraphicsEngine SHARED
    src/Window.cpp src/GraphicsEngine.cpp
    src/ImGuiBuild.cpp src/Gui.cpp
    src/Material.cpp
    src/Texture.cpp
    src/TextureInternal.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Input.cpp
)
add_library (imgui SHARED imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui_widgets.cpp)
