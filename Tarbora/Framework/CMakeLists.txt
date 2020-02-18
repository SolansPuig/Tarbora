find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(ProtoSources ProtoHeaders MessageManager/messages.proto)

add_subdirectory (External)

add_library (TarboraCore SHARED
    Logger.cpp
    Module/AbstractModule.cpp
    Module/Module.cpp
    Module/ModuleComponent.cpp
    ResourceManager/ResourceManager.cpp
    ResourceManager/Resource.cpp
    Math/Math.cpp
    MessageManager/MessageClient.cpp
    MessageManager/MessageHub.cpp
    MessageManager/MessageManager.cpp
    ${ProtoSources}
    ${ProtoHeaders}
)

target_link_libraries(TarboraCore PUBLIC lua tracy glm grpc grpc++ grpc++_reflection)
target_compile_options(TarboraCore PRIVATE -Werror -Wall -Wextra -Wpedantic)
