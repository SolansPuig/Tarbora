cmake_minimum_required(VERSION 2.8)
project (Tarbora)

set (CMAKE_BUILD_TYPE Debug)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-Wall -Wextra -Ofast")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -D DEBUG_MODE=true")
    set (CMAKE_CXX_FLAGS_RELEASE "-D DEBUG_MODE=false")
    set (CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLU -lX11 -lpthread -lXrandr -ldl -lGLEW")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Tarbora/Framework/PhysicsEngine/bullet3/src)
include_directories(Tarbora/Framework/GraphicsEngine/imgui)
include_directories(Tarbora/Framework/GraphicsEngine/glfw/include)

# add_subdirectory (Demo)
add_subdirectory (Tarbora)

add_executable (tarbora main.cpp)
target_link_libraries (tarbora Application)
target_link_libraries (tarbora GameView)
target_link_libraries (tarbora GameLogic)
target_link_libraries (tarbora GraphicsEngine)
target_link_libraries (tarbora PhysicsEngine)
target_link_libraries (tarbora ResourceManager)
target_link_libraries (tarbora EventManager)
target_link_libraries (tarbora Logger)

target_link_libraries (tarbora BulletDynamics)
target_link_libraries (tarbora BulletCollision)
target_link_libraries (tarbora LinearMath)

target_link_libraries (tarbora glfw)
target_link_libraries (tarbora imgui)

target_link_libraries (tarbora glm)
