add_subdirectory (PhysicsEngine)
add_subdirectory (External)

add_library (TarboraCore SHARED
    AbstractModule.cpp
    Module.cpp
    ModuleComponent.cpp
    Logger.cpp
    MessageManager.cpp
    MessageClient.cpp
    MessageHub.cpp
    Protos/messages.grpc.pb.cc
    Protos/messages.pb.cc
    External/tracy/TracyClient.cpp
    ResourceManager/src/ResourceManager.cpp
    ResourceManager/src/Resource.cpp
    ResourceManager/src/Json.cpp
)
