#include "Tarbora/Views/GraphicViews/HumanView.hpp"
#include "Tarbora/Views/SceneManager/SceneManager.hpp"
#include "Tarbora/Views/HardwareViews/inc/ArduinoView.hpp"
#include "Tarbora/Logic/GameLogic.hpp"
#include "Tarbora/Messages/BasicMessages.hpp"
#include "Tarbora/Editor/Editor.hpp"

using namespace Tarbora;

int main() {
    ZoneScoped;
    LOG_LEVEL(DEBUG);

    ResourceManager::init("../Resources/");
    
    // auto message_bus = std::make_shared<MessageBus>("0.0.0.0:50051");
    // message_bus->runThread("Message Bus");

    auto logic = std::make_shared<World>();
    logic->registerSystem(std::make_shared<InventorySystem>(logic.get()));
    logic->registerSystem(std::make_shared<RenderSystem>(logic.get()));
    logic->registerSystem(std::make_shared<AnimationSystem>(logic.get()));
    logic->registerSystem(std::make_shared<EntitySystem>(logic.get()));
    logic->registerSystem(std::make_shared<PhysicsSystem>(logic.get()));
    logic->registerSystem(std::make_shared<ControllerSystem>(logic.get()));
    logic->registerSystem(std::make_shared<GrabSystem>(logic.get()));

    logic->runThread("Logic");

    // auto arduino = std::make_shared<ArduinoView>();
    // arduino->runThread("Arduino");

    auto human_view = std::make_shared<HumanView>();
    human_view->pushLayer(std::make_shared<Editor>(human_view.get(), false));

    auto scene_manager = std::make_shared<SceneManager>();
    scene_manager->load("test.lua");

    human_view->run("Human View");

    ResourceManager::close();
    return 0;
}
