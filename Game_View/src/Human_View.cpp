#include "Human_View.hpp"
#include "Graphics_Engine.hpp"

namespace Tarbora {
    Human_View::Human_View(float r, float g, float b)
    {
        Graphics_Engine::GetWindow()->SetClearColor(r, g, b);
    }

    Human_View::~Human_View()
    {
    }

    void Human_View::Update(float elapsed_time)
    {}

    void Human_View::Draw()
    {
        Graphics_Engine::BeforeDraw();

        for (auto &itr : m_Layers)
        {
            if (itr->IsActive())
                itr->Draw();
        }

        Graphics_Engine::AfterDraw();
    }

    void Human_View::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.push_back(layer);
    }

    void Human_View::RemoveLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.remove(layer);
    }
}
