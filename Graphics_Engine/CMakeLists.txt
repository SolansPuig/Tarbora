include_directories(inc/)
add_subdirectory (glfw)
add_subdirectory (imgui)
add_library (Graphics_Engine SHARED src/Window.cpp src/Graphics_Engine.cpp src/ImGuiBuild.cpp src/Gui.cpp)
