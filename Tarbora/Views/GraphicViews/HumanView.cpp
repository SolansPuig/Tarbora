#include "HumanView.hpp"
#include "DemoWindow.hpp"
#include "GameLayer.hpp"
#include "MetricsGui.hpp"
#include "InventoryLayer.hpp"

namespace Tarbora {
    HumanView::HumanView() :
        GraphicView(20, "Settings.lua")
    {
        LOG_DEBUG("Creating human game view...");

        max_fps_ = 60;

        game_layer_ = std::shared_ptr<GameLayer>(new GameLayer(this, true));
        game_layer_->setTargetId("player");
        pushLayer(game_layer_);

        std::shared_ptr<MetricsGui> metrics(new MetricsGui(this, false));
        pushLayer(metrics);

        std::shared_ptr<DemoWindow> demo_gui(new DemoWindow(this, false));
        pushLayer(demo_gui);

        std::shared_ptr<InventoryLayer> inventory(new InventoryLayer(this));
        pushLayer(inventory);

        getGraphicsEngine()->getWindow()->captureMouse(true);

        LOG_DEBUG("Created");
    }

    HumanView::~HumanView()
    {
        LOG_DEBUG("Destroying human game view...");
    }

    void HumanView::getInput()
    {
        ZoneScoped;

        if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_ESCAPE)) {
            static bool capture = true;
            capture = !capture;
            getGraphicsEngine()->getWindow()->captureMouse(capture);
            game_layer_->freezeMouse(!capture);
        }

        if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F2)) {
            getGraphicsEngine()->getWindow()->takeScreenshot("/home/roger/Imatges/test");
        }

        if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F5)) {
            ResourceManager::flush();
        }

        for (auto itr = layers_.rbegin(); itr != layers_.rend(); itr++)
        {
            (*itr)->getInput();
        }
    }

    void HumanView::update(float delta_time)
    {
        ZoneScoped;

        for (auto itr = layers_.rbegin(); itr != layers_.rend(); itr++)
        {
            if ((*itr)->isActive())
                (*itr)->update(delta_time);
        }
    }

    void HumanView::draw()
    {
        {
            ZoneScopedN("Before Draw");
            getGraphicsEngine()->beforeDraw();
        }
        {
            ZoneScopedN("Draw");
            for (auto &itr : layers_)
            {
                if (itr->isActive())
                    itr->draw();
            }
        }
        {
            ZoneScopedN("After Draw");
            getGraphicsEngine()->afterDraw();
        }
    }

    void HumanView::pushLayer(std::shared_ptr<Layer> layer)
    {
        layers_.push_back(layer);
    }

    void HumanView::removeLayer(std::shared_ptr<Layer> layer)
    {
        layers_.remove(layer);
    }
}
