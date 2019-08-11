add_library (MessageManager SHARED
    src/MessageManager.cpp
    src/NetworkClient.cpp
    src/NetworkServer.cpp
    src/MathMessages.cpp
    src/BasicMessages.cpp
    proto/messages.grpc.pb.cc
    proto/messages.pb.cc
    proto/basic.pb.cc proto/math.pb.cc
    ../Application.cpp
)
