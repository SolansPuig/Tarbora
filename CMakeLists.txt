cmake_minimum_required(VERSION 2.8)
project (Tarbora)

set (CMAKE_BUILD_TYPE Debug)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-Ofast -DTRACY_ENABLE -fPIC")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -D DEBUG_MODE=true")
    set (CMAKE_CXX_FLAGS_RELEASE "-D DEBUG_MODE=false")
    set (CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLU -lX11 -lpthread -lXrandr -ldl -lGLEW")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include(cotire)

include_directories(SYSTEM Tarbora/Framework/External/glm)
include_directories(SYSTEM Tarbora/Framework/External/lua)
include_directories(SYSTEM Tarbora/Framework/External/sol2/include)
include_directories(SYSTEM Tarbora/Framework/External/tracy)
include_directories(SYSTEM Tarbora/Logic/PhysicsEngine/External/bullet3/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Tarbora/Messages)
include_directories(SYSTEM Tarbora/Framework/External/glm)
include_directories(SYSTEM Tarbora/Views/GraphicViews/GraphicsEngine/External/imgui)
include_directories(SYSTEM Tarbora/Views/GraphicViews/GraphicsEngine/External/glfw/include)

add_subdirectory (Tarbora)

add_executable (tarbora main.cpp)

target_link_libraries (tarbora
    TarboraCore
    Messages
    Logic
    GraphicViews
    HardwareViews
    SceneManager
    PhysicsEngine
    GraphicsEngine
    Editor
)
