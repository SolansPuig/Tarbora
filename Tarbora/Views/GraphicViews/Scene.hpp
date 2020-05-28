#ifndef __SCENE_H_
#define __SCENE_H_

#include "SceneNode.hpp"
#include "ActorModel.hpp"
#include "Skybox.hpp"

namespace Tarbora {
  class HumanView;

  class Scene
  {
  public:
    Scene(HumanView *view);

    void setWindowRatio(float ratio);
    void update(float delta_time);
    void draw();

    std::shared_ptr<ActorModel> createActorModel(
      const ActorId &id, RenderPass render_pass,
      const std::string &model, const std::string &material
    );

    std::shared_ptr<Skybox> createSkybox(const std::string &material);
    std::shared_ptr<Skybox> getSkybox() { return skybox_; }

    void addActor(std::shared_ptr<SceneNode> actor);
    std::shared_ptr<SceneNode> getActor(const ActorId &id);
    bool removeActor(const ActorId &id);

    std::shared_ptr<Camera> createCamera(const ActorId &id);
    void setCamera(std::shared_ptr<Camera> camera) { camera_ = camera; }
    void setCamera(const ActorId &id, const std::string &node_name);
    std::shared_ptr<Camera> getCamera() { return camera_; }

    inline std::shared_ptr<GraphicsEngine> getGraphicsEngine()
    {
      return view_->getGraphicsEngine();
    }
    inline std::shared_ptr<RenderQueue> getRenderQueue()
    {
      return view_->getGraphicsEngine()->getRenderQueue();
    }

    inline const glm::mat4& getView()
    {
      return view_matrix_;
    }

    inline const glm::mat4 & getProjection()
    {
      return projection_;
    }

  protected:
    std::shared_ptr<RootNode> root_;
    std::shared_ptr<Skybox> skybox_;
    std::shared_ptr<Camera> camera_;

    std::map<ActorId, std::shared_ptr<SceneNode>> actor_map_;

    glm::mat4 projection_;
    glm::mat4 view_matrix_;

    GraphicView *view_;
  };
}

#endif // __SCENE_H_
