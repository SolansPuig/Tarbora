#include "Scene.hpp"
#include <utility>

namespace Tarbora {
    Scene::Scene(HumanView *view) :
        view_(view)
    {
        root_ = std::shared_ptr<RootNode>(new RootNode());
    }

    void Scene::update(float delta_time)
    {
        if (root_)
            root_->update(this, delta_time);
    }

    void Scene::draw()
    {
        projection_ = glm::perspective(glm::radians(45.0f), getGraphicsEngine()->getWindow()->getRatio(), 0.1f, 100.0f);
        getRenderQueue()->setProjectionMatrix(projection_);
        getRenderQueue()->setViewMatrix(camera_->getView());

        if (root_)
        {
            root_->drawChildren(this, glm::mat4(1.0f));
            getRenderQueue()->draw();
        }
    }

    std::shared_ptr<Skybox> Scene::createSkybox(const std::string &material)
    {
        skybox_ = std::shared_ptr<Skybox>(new Skybox(material));
        addActor(skybox_);
        return skybox_;
    }

    std::shared_ptr<ActorModel> Scene::createActorModel(const ActorId &id, RenderPass render_pass, const std::string &model, const std::string &material)
    {
        if (getActor(id))
            removeActor(id);

        std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, render_pass, model, material));
        addActor(actor);
        return actor;
    }

    std::shared_ptr<Camera> Scene::createCamera(const ActorId &id)
    {
        std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera(id, "body"));
        if (camera)
            addActor(camera);
        return camera;
    }

    void Scene::setCamera(const ActorId &id, const std::string &name)
    {
        std::shared_ptr<SceneNode> child = getActor(id);
        if (child)
        {
            std::shared_ptr<SceneNode> camera = child->getChild(name);
            if (camera)
            {
                camera_ = std::static_pointer_cast<Camera>(camera);
            }
        }
    }

    void Scene::addActor(std::shared_ptr<SceneNode> actor)
    {
        const ActorId &id = actor->getActorId();
        if (id != "")
            actor_map_[id] = actor;
        root_->addChild(actor);
    }

    std::shared_ptr<SceneNode> Scene::getActor(const ActorId &id)
    {
        auto itr = actor_map_.find(id);
        if (itr == actor_map_.end())
        {
            return std::shared_ptr<SceneNode>();
        }
        return itr->second;
    }

    bool Scene::removeActor(const ActorId &id)
    {
        if (id != "")
        {
            actor_map_.erase(id);
            return root_->removeChild(id);
        }
        return false;
    }
}
