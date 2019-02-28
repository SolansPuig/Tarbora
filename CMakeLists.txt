cmake_minimum_required(VERSION 2.8)
project (Tarbora)

set (CMAKE_BUILD_TYPE Debug)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Ofast")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -D DEBUG=true")
    set (CMAKE_CXX_FLAGS_RELEASE "-D DEBUG=false")
    set (CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLU -lX11 -lpthread -lXrandr -ldl -lGLEW")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories (utils/)
include_directories (glfw/include)

add_subdirectory (Graphics_Engine)
add_subdirectory (glfw)

add_executable (tarbora main.cpp)
target_link_libraries (tarbora Graphics_Engine)
target_link_libraries (tarbora glfw)
