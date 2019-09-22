add_subdirectory (ResourceManager)
add_subdirectory (PhysicsEngine)
add_subdirectory (External)

add_library (TarboraCore SHARED
    AbstractModule.cpp
    Module.cpp
    ModuleComponent.cpp
    Logger.cpp
    MessageManager.cpp
    MessageClient.cpp
    MessageServer.cpp
    Protos/messages.grpc.pb.cc
    Protos/messages.pb.cc
)
