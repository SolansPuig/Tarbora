#include "Tarbora/Tarbora.hpp"
#include "Demo/inc/DemoWindow.hpp"
#include "Demo/inc/MetricsGui.hpp"
#include "Demo/inc/TestGui.hpp"

namespace Tb = Tarbora;

int main() {
    std::unique_ptr<Tb::Application> app(new Tb::Application());
    Tb::HumanViewPtr human_view(new Tb::HumanView(1));
    app->AddView(human_view);

    std::shared_ptr<MetricsGui> metrics(new MetricsGui(false));
    human_view->PushLayer(metrics);

    std::shared_ptr<TestGui> test_gui(new TestGui(false));
    human_view->PushLayer(test_gui);

    // std::shared_ptr<DemoWindow> demo_gui(new DemoWindow());
    // human_view->PushLayer(demo_gui);

    Tb::EventManager::Subscribe(Tb::EventType::WindowClose, [&](Tb::Event *e)
    {
        (void)(e);
        app->Close();
    });

    Tb::EventManager::Subscribe(Tb::EventType::KeyRelease, [&](Tb::Event *ev)
    {
        Tb::KeyReleaseEvent *e = static_cast<Tb::KeyReleaseEvent*>(ev);
        if (e->key == KEY_F3) metrics->SetActive(!metrics->IsActive());
        else if (e->key == KEY_F2) Tb::GraphicsEngine::TakeScreenshot("/home/roger/Imatges/test");
        else if (e->key == KEY_ESCAPE) app->Close();
        else if (e->key == KEY_X)
        {
            Tb::CreateActorEvent ev = Tb::CreateActorEvent("entities/cube.json", glm::vec3(0.f, 5.f, -5.f));
            Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
        }
        else if (e->key == KEY_Z)
        {
            Tb::CreateActorEvent ev = Tb::CreateActorEvent("entities/ball.json", glm::vec3(0.f, 5.f, -5.f));
            Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
        };
    });

    Tb::CreateActorEvent ev = Tb::CreateActorEvent("entities/ball.json", glm::vec3(-2.f, 5.f, -5.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/cube.json", glm::vec3(0.5f, 5.f, -5.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/cube.json", glm::vec3(0.f, 5.f, -5.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/ground.json", glm::vec3(0.f, -3.f, -5.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/ground.json", glm::vec3(-5.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 90.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/ground.json", glm::vec3(0.f, -3.f, -10.f), glm::vec3(90.f, 0.f, 0.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/ground.json", glm::vec3(0.f, -6.5f, 0.f), glm::vec3(90.f, 0.f, 0.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);
    ev = Tb::CreateActorEvent("entities/ground.json", glm::vec3(5.f, -3.f, -5.f), glm::vec3(0.f, 0.f, 90.f));
    Tb::EventManager::Trigger(Tb::EventType::CreateActor, &ev);

    app->Run();

    return 0;
}
