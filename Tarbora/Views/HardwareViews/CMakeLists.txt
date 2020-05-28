add_library (HardwareViews SHARED ArduinoView.cpp)
add_dependencies (HardwareViews Messages)
target_compile_options(HardwareViews PRIVATE -Werror -Wall -Wextra -Wpedantic)
