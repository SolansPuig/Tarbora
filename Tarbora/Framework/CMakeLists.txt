add_subdirectory (MessageManager)
add_subdirectory (ResourceManager)
add_subdirectory (PhysicsEngine)
add_subdirectory (Maths)
add_subdirectory (Utility)

add_library (Application SHARED ClientApplication.cpp Application.cpp)
