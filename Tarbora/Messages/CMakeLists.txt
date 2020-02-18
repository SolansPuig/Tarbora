find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(ProtoSources ProtoHeaders math.proto basic.proto)
add_library (Messages SHARED
    ${ProtoSources}
    ${ProtoHeaders}
)
