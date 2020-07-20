/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#ifndef __EDITOR_H_
#define __EDITOR_H_

#include "../Views/GraphicViews/Layer.hpp"
#include <ctime>

namespace Tarbora {
  class HumanView;
  class ActorModel;
  class Scene;
  class SceneNode;
  class ModelEditor;
  class NodeEditor;
  class GameLayer;

  class Editor : public Layer
  {
  public:
    Editor(HumanView *view, bool startActive, GameLayer *layer);

    virtual bool getInput();

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
    NodeEditor(Editor *editor, std::shared_ptr<Scene> scene);

    void setTarget(const std::string &name, std::shared_ptr<SceneNode> node);

    void draw(bool *active);

  private:
    Editor *editor_;
    std::shared_ptr<Scene> scene_;

    std::string node_name_;
    std::weak_ptr<SceneNode> node_;
    bool auto_texture_size_;
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

#endif // __EDITOR_H_
