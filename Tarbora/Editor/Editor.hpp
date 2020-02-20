#pragma once
#include "../Views/GraphicViews/Layer.hpp"
#include <ctime>

namespace Tarbora {
    class HumanView;
    class ActorModel;
    class Scene;
    class SceneNode;
    class ModelEditor;
    class NodeEditor;

    class Editor : public Layer
    {
    public:
        Editor(HumanView *view, bool startActive);

        virtual void getInput();

        virtual void draw();

        std::shared_ptr<ModelEditor> model_editor;
        std::shared_ptr<NodeEditor> node_editor;

    private:
        std::shared_ptr<Scene> scene_;
    };

    class ModelEditor
    {
    public:
        ModelEditor(Editor *editor, std::shared_ptr<Scene> scene);

        void setTarget(const ActorId &id);

        void draw(bool *active);

        void nodeInspector(std::shared_ptr<SceneNode> node);

    private:
        Editor *editor_;
        std::shared_ptr<Scene> scene_;

        ActorId actor_id_;
        bool captured_;
        std::weak_ptr<ActorModel> model_;
        std::weak_ptr<SceneNode> selected_node_;

        std::weak_ptr<SceneNode> edited_node_;
    };

    class NodeEditor
    {
    public:
        NodeEditor(Editor *editor);

        void setTarget(const std::string &name, std::shared_ptr<SceneNode> node);

        void draw(bool *active);

    private:
        Editor *editor_;

        std::string node_name_;
        std::weak_ptr<SceneNode> node_;
    };

    class ModelSaver
    {
    public:
        ModelSaver(const std::string &file, std::shared_ptr<ActorModel> model);

    private:
        void saveNode(std::shared_ptr<SceneNode> node);
        void saveVec3(const std::string &name, const glm::vec3 &vector);
        void saveVec2(const std::string &name, const glm::vec2 &vector);
        void saveVec3(const std::string &name, const glm::tvec3<unsigned char> &vector);
        void saveVec2(const std::string &name, const glm::tvec2<unsigned short> &vector);

        std::ofstream file_;
        unsigned int indentation_;
    };
}
