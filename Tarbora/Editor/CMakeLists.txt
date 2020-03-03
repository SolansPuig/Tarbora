add_library (Editor SHARED
    ModelEditor.cpp
)
add_dependencies (Editor Messages)
target_compile_options(Editor PRIVATE -Werror -Wall -Wextra -Wpedantic)
