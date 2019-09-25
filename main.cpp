#include "Tarbora/Framework/MessageBus.hpp"
#include "Tarbora/Views/GraphicViews/inc/HumanView.hpp"
#include "Tarbora/Views/HardwareViews/inc/ArduinoView.hpp"
#include "Tarbora/Logic/inc/World.hpp"
#include "Tarbora/Messages/BasicMessages.hpp"

using namespace Tarbora;

int main() {
    ZoneScoped;
    LOG_LEVEL(DEBUG);

    std::shared_ptr<MessageBus> messageBus(new MessageBus("0.0.0.0:50051"));
    messageBus->RunThread("Message Bus");

    std::shared_ptr<World> logic(new World("localhost:50051", 1000));
    logic->RunThread("Logic");

    // std::shared_ptr<ArduinoView> arduino(new ArduinoView("localhost:50051"));
    // arduino->RunThread("arduino");

    std::shared_ptr<HumanView> humanView(new HumanView("localhost:50051"));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "ground.json", "", glm::vec3(0.f, -3.f, -5.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "ground.json", "", glm::vec3(-4.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 90.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "ground.json", "", glm::vec3(0.f, -2.5f, -60.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "ground.json", "", glm::vec3(4.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 45.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "human.json", "", glm::vec3(0.f, 0.f, -5.f)));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,3.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,4.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,5.f,-5.f));

    humanView->Run("Human View");

    return 0;
}
