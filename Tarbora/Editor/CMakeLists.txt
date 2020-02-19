add_library (Editor SHARED
    ModelEditor.cpp
)
target_compile_options(Editor PRIVATE -Werror -Wall -Wextra -Wpedantic)
