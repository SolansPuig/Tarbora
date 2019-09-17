add_subdirectory (MessageManager)
add_subdirectory (ResourceManager)
add_subdirectory (PhysicsEngine)
add_subdirectory (External)

add_library (TarboraCore SHARED
    AbstractModule.cpp
    Module.cpp
    Logger.cpp
)
