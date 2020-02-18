#include "Tarbora/Views/GraphicViews/HumanView.hpp"
#include "Tarbora/Views/SceneManager/SceneManager.hpp"
#include "Tarbora/Views/HardwareViews/inc/ArduinoView.hpp"
#include "Tarbora/Logic/World.hpp"
#include "Tarbora/Messages/BasicMessages.hpp"

using namespace Tarbora;

int main() {
    ZoneScoped;
    LOG_LEVEL(DEBUG);
    ResourceManager::init("../Resources/");

    // std::shared_ptr<MessageBus> messageBus(new MessageBus("0.0.0.0:50051"));
    // messageBus->RunThread("Message Bus");

    std::shared_ptr<World> logic(new World());
    logic->runThread("Logic");

    // std::shared_ptr<ArduinoView> arduino(new ArduinoView());
    // arduino->RunThread("arduino");

    std::shared_ptr<HumanView> human_view(new HumanView());

    std::shared_ptr<SceneManager> scene_manager(new SceneManager());
    scene_manager->load("test.lua");

    human_view->run("Human View");

    ResourceManager::close();

    return 0;
}
