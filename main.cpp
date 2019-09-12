#include "Tarbora/Framework/MessageManager/inc/NetworkServer.hpp"
#include "Tarbora/Views/GraphicViews/inc/HumanView.hpp"
#include "Tarbora/Views/HardwareViews/inc/ArduinoView.hpp"
#include "Tarbora/Logic/inc/Actors.hpp"

using namespace Tarbora;

int main() {
    LOG_LEVEL(DEBUG);

    std::shared_ptr<NetworkServer> server(new NetworkServer("0.0.0.0:50051"));
    server->RunThread();

    std::shared_ptr<Actors> actors(new Actors("localhost:50051", 1000));
    actors->RunThread();

    // std::shared_ptr<ArduinoView> arduino(new ArduinoView("localhost:50051"));
    // arduino->RunThread();

    std::shared_ptr<HumanView> human_view(new HumanView("localhost:50051"));
    CreateActor(human_view.get(), "ground.json", "", glm::vec3(0.f, -3.f, -5.f));
    CreateActor(human_view.get(), "ground.json", "", glm::vec3(-4.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 90.f));
    CreateActor(human_view.get(), "ground.json", "", glm::vec3(0.f, -2.f, -60.f));
    CreateActor(human_view.get(), "ground.json", "", glm::vec3(4.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 45.f));
    CreateActor(human_view.get(), "human.json", "", glm::vec3(0.f, 0.f, -5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,3.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,4.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,5.f,-5.f));

    human_view->Run();

    return 0;
}
