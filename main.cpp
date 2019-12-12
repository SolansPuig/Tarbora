#include "Tarbora/Framework/MessageBus.hpp"
#include "Tarbora/Views/GraphicViews/inc/HumanView.hpp"
#include "Tarbora/Views/HardwareViews/inc/ArduinoView.hpp"
#include "Tarbora/Logic/inc/World.hpp"
#include "Tarbora/Messages/BasicMessages.hpp"

using namespace Tarbora;

int main() {
    ZoneScoped;
    LOG_LEVEL(DEBUG);

    // std::shared_ptr<MessageBus> messageBus(new MessageBus("0.0.0.0:50051"));
    // messageBus->RunThread("Message Bus");

    std::shared_ptr<World> logic(new World(1000));
    logic->RunThread("Logic");

    // std::shared_ptr<ArduinoView> arduino(new ArduinoView());
    // arduino->RunThread("arduino");

    std::shared_ptr<HumanView> humanView(new HumanView());
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "ground.json", "", glm::vec3(0.f, -3.f, -5.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "platform.json", "", glm::vec3(-3.f, -1.5f, -7.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "platform.json", "", glm::vec3(-5.f, -1.5f, -7.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "platform.json", "", glm::vec3(-5.f, -1.5f, -5.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "double_platform.json", "", glm::vec3(-5.f, (-2.f) - (1.34f), -4.f), glm::vec3(45.f, 0.f, 0.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "human.json", "", glm::vec3(0.f, -1.f, -3.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "birch.json", "", glm::vec3(2.f, -1.5f, -5.f)));
    humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "birch.json", "", glm::vec3(3.f, -1.5f, -2.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(0.f, 3.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.1f, 4.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.2f, 5.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.3f, 6.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.1f, 7.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.2f, 8.f, -5.f)));
    // humanView->GetMessageManager()->Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(-0.3f, 9.f, -5.f)));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,3.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,4.f,-5.f));
    // CreateActor(human_view.get(), "cube.json", "", glm::vec3(0.f,5.f,-5.f));

    humanView->Run("Human View");

    return 0;
}
