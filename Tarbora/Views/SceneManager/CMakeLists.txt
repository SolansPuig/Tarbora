add_library (SceneManager SHARED SceneManager.cpp)
add_dependencies (SceneManager Messages)
target_compile_options(SceneManager PRIVATE -Werror -Wall -Wextra -Wpedantic)
