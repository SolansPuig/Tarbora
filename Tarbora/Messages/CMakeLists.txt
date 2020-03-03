find_package(Protobuf 3 REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(ProtoSources ProtoHeaders math.proto basic.proto lua.proto)
add_library (Messages STATIC
    ${ProtoSources}
    ${ProtoHeaders}
    BasicMessages.cpp
)
add_dependencies (Messages TarboraCore)
