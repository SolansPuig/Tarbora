#include "Tarbora.hpp"
#include "Demo_Window.hpp"
#include "Metrics_Gui.hpp"
#include "Test_Gui.hpp"

namespace Tb = Tarbora;

int main() {
    Tb::Application* app = new Tb::Application();
    Tb::HumanViewPtr human_view(new Tb::Human_View(0, 0, 0));
    app->AddView(human_view);

    std::shared_ptr<Metrics_Gui> metrics(new Metrics_Gui());
    human_view->PushLayer(metrics);
    metrics->SetActive(false);

    std::shared_ptr<Test_Gui> test_gui(new Test_Gui());
    human_view->PushLayer(test_gui);
    std::shared_ptr<Demo_Window> demo_gui(new Demo_Window());
    human_view->PushLayer(demo_gui);

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
