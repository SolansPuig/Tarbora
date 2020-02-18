add_library (HardwareViews SHARED src/ArduinoView.cpp)
target_compile_options(HardwareViews PRIVATE -Werror -Wall -Wextra -Wpedantic)
