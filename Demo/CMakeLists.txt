add_library(Demo SHARED
  GravityRandomizerSystem.cpp
  KnockbackSystem.cpp
  ThrowSystem.cpp
  Cockatiel.cpp
)
target_link_libraries(Demo Messages)
target_compile_options(Demo PRIVATE -Werror -Wall -Wextra -Wpedantic)
