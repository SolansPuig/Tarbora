#include "Tarbora/Framework/MessageManager/inc/NetworkServer.hpp"

using namespace Tarbora;

int main() {
    LOG_LEVEL(DEBUG);

    std::shared_ptr<NetworkServer> server(new NetworkServer("0.0.0.0:50051"));

    server->Run();

    return 0;
}
