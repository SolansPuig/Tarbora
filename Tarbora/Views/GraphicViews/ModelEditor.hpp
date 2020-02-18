#pragma once
#include "Layer.hpp"
#include <ctime>

namespace Tarbora {
    class HumanView;
    class ActorModel;
    class Scene;
    class SceneNode;

    class ModelEditor : public Layer
    {
    public:
        ModelEditor(HumanView *view, bool startActive);

        virtual void getInput();

        virtual void draw();

        void nodeInspector(std::shared_ptr<SceneNode> node);

    private:
        ActorId actor_id_;
        bool captured_;
        std::shared_ptr<Scene> scene_;
        std::shared_ptr<ActorModel> model_;
        std::shared_ptr<SceneNode> selected_node_;
    };
}
