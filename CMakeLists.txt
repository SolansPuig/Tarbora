cmake_minimum_required(VERSION 2.8)
project (Tarbora)

set (CMAKE_BUILD_TYPE Debug)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Ofast")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -D DEBUG_MODE=true")
    set (CMAKE_CXX_FLAGS_RELEASE "-D DEBUG_MODE=false")
    set (CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLU -lX11 -lpthread -lXrandr -ldl -lGLEW")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Tarbora/Framework/PhysicsEngine/bullet3/src)
include_directories(Tarbora/Views/GraphicsEngine/imgui)
include_directories(Tarbora/Views/GraphicsEngine/glfw/include)

add_subdirectory (Tarbora)

add_executable (tarbora main.cpp)
target_link_libraries (tarbora
    GraphicViews
    HardwareViews
    Application
    Logic
    PhysicsEngine
    GraphicsEngine
    ResourceManager
    MessageManager
    Logger
    glfw
    imgui
    glm
    grpc
    grpc++
    grpc++_reflection
    BulletDynamics
    BulletCollision
    LinearMath
)

add_executable (tarbora_client main_client.cpp)
target_link_libraries (tarbora_client
    GraphicViews
    Application
    GraphicsEngine
    ResourceManager
    MessageManager
    Logger
    glfw
    imgui
    glm
    grpc
    grpc++
    grpc++_reflection
)

add_executable (tarbora_console console_client.cpp)
target_link_libraries (tarbora_console
    GraphicViews
    Application
    GraphicsEngine
    ResourceManager
    MessageManager
    Logger
    glfw
    imgui
    grpc
    grpc++
    grpc++_reflection
)

add_executable (tarbora_server server.cpp)
target_link_libraries (tarbora_server
    MessageManager
    Logger
    grpc
    grpc++
    grpc++_reflection
)


# add_executable (tarbora_logic main_logic.cpp)
# target_link_libraries (tarbora_logic GameLogic)
# target_link_libraries (tarbora_logic PhysicsEngine)
# target_link_libraries (tarbora_logic ResourceManager)
# target_link_libraries (tarbora_logic EventManager)
# target_link_libraries (tarbora_logic Logger)
#
# target_link_libraries (tarbora_logic BulletDynamics)
# target_link_libraries (tarbora_logic BulletCollision)
# target_link_libraries (tarbora_logic LinearMath)
#
# target_link_libraries (tarbora_logic glm)
