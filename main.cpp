#include "Tarbora.hpp"
#include "DemoWindow.hpp"
#include "MetricsGui.hpp"
#include "TestGui.hpp"

namespace Tb = Tarbora;

int main() {
    Tb::Application* app = new Tb::Application("Settings.json");
    Tb::HumanViewPtr human_view(new Tb::HumanView(MAIN_CAMERA_ID));
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
        else if (e->key == KEY_ESCAPE) app->Close();
    });

    app->Run();

    delete app;
    return 0;
}
