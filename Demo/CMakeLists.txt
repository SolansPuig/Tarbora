add_library(Demo SHARED
  GravityRandomizerSystem.cpp
  KnockbackSystem.cpp
  ThrowSystem.cpp
)
target_compile_options(Demo PRIVATE -Werror -Wall -Wextra -Wpedantic)
