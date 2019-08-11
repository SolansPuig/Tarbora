#include "Tarbora/Framework/MessageManager/inc/NetworkServer.hpp"

using namespace Tarbora;

int main() {
    Logger::Init(stdout);
    LOG_LEVEL(Logger::LogLevel::DEBUG);

    std::shared_ptr<NetworkServer> server(new NetworkServer("0.0.0.0:50051"));

    server->Run();

    Logger::Close();
    return 0;
}
