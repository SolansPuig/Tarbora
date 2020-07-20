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

// TODO: This file will change a lot soon

#include "Editor.hpp"
#include "../Views/GraphicViews/HumanView.hpp"
#include "../Views/GraphicViews/GameLayer.hpp"
#include "../Views/GraphicViews/ActorModel.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
  Editor::Editor(HumanView *view, bool start_active, GameLayer *layer)
    : Layer(view, start_active)
  {
    scene_ = layer->getScene();

    model_editor = std::shared_ptr<ModelEditor>(new ModelEditor(this, scene_));
    node_editor = std::shared_ptr<NodeEditor>(new NodeEditor(this, scene_));

    model_editor->setTarget("");

    subscribe("look_at", [&](const MessageSubject, const MessageBody &body)
    {
      Message::LookAt m(body);
      if (m.getId() == "player")
        model_editor->setTarget(m.getTarget());
    });
  }

  bool Editor::getInput()
  {
    if (getInputManager()->getKeyDown(KEY_F4)) {
      active_ = !active_;
    }

    return true;
  }

  void Editor::draw()
  {
    if (active_)
    {
      model_editor->draw(&active_);
      node_editor->draw(&active_);
    }
  }

  ModelEditor::ModelEditor(Editor *editor, std::shared_ptr<Scene> scene)
  {
    editor_ = editor;
    scene_ = scene;
    captured_ = false;
  }

  void ModelEditor::setTarget(const ActorId &id)
  {
    if (!captured_)
    {
      if (actor_id_ != id)
      {
        actor_id_ = id != "" ? id : "sky";
        model_ = std::static_pointer_cast<ActorModel>(scene_->getActor(actor_id_));
        editor_->node_editor->setTarget("", std::shared_ptr<SceneNode>());
      }
    }
  }

  void ModelEditor::draw(bool *active)
  {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
    ImGui::Begin(ICON_FK_PAINT_BRUSH " Model Editor", active, ImGuiWindowFlags_None);

    // Header
    ImGui::BeginGroup();
    ImGui::Text("Target: %s ", actor_id_.c_str());

    ImGui::SameLine(400.f);
    if (actor_id_ != "") ImGui::Checkbox("Captured", &captured_);
    ImGui::EndGroup();

    ImGui::Separator();

    // Inspector
    const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("Inspector", ImVec2(0, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (auto model = model_.lock())
    {
      // Options
      if (ImGui::CollapsingHeader("Options"))
      {
        ImGui::Spacing();

        if (ImGui::Button("Reload"))
        {
          // TODO: Change that
          glm::vec3 position = model->getPosition();
          glm::quat rotation = model->getOrientation();
          ResourceManager::flush();
          model_ = scene_->createActorModel(actor_id_, model->getRenderPass(), model->getModel(), model->getMaterial());
          model = model_.lock();
          model->setPosition(position);
          model->setOrientation(rotation);
        }

        ImGui::SameLine();

        if (ImGui::Button("Load from..."))
        {
          ImGui::OpenPopup("Select File");
        }

        ImGui::SameLine();

        if (ImGui::Button("Save"))
        {
          ImGui::OpenPopup("Save to File");
        }

        if (ImGui::BeginPopupModal("Select File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
          editor_->getInputManager()->enableInput(false);
          static std::string file = "example.lua";
          ImGui::InputText("File Name", &file);

          ImGui::Separator();

          // Submit or cancel the modal
          if (ImGui::Button("Cancel", ImVec2(120, 0)))
          {
            file = "example.lua";
            editor_->getInputManager()->enableInput(true);
            ImGui::CloseCurrentPopup();
          }
          ImGui::SameLine();
          if (ImGui::Button("Ok", ImVec2(120, 0)) && file != "")
          {
            // TODO: Change that
            glm::vec3 position = model->getPosition();
            glm::quat rotation = model->getOrientation();
            ResourceManager::flush();
            model_ = scene_->createActorModel(actor_id_, model->getRenderPass(), file, model->getMaterial());
            model = model_.lock();
            model->setPosition(position);
            model->setOrientation(rotation);

            file = "example.lua";
            ImGui::CloseCurrentPopup();
            editor_->getInputManager()->enableInput(true);
          }
          ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Save to File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
          editor_->getInputManager()->enableInput(false);
          ImGui::TextUnformatted("Caution! This function could overwrite files that have extended functionality, losing it. Always keep a copy!");
          static std::string file = "example.lua";
          ImGui::InputText("File Name", &file);

          ImGui::Separator();

          // Submit or cancel the modal
          if (ImGui::Button("Cancel", ImVec2(120, 0)))
          {
            file = "example.lua";
            ImGui::CloseCurrentPopup();
          }
          ImGui::SameLine();
          if (ImGui::Button("Ok", ImVec2(120, 0)) && file != "")
          {
            ModelSaver saver("../Resources/models/" + file, model);
            file = "example.lua";
            ImGui::CloseCurrentPopup();
            editor_->getInputManager()->enableInput(true);
          }
          ImGui::EndPopup();
        }

        ImGui::Spacing();

        const float step = 0.1f;
        float scale = model->getGlobalScale();
        if (ImGui::InputScalar("Global Scale", ImGuiDataType_Float, &scale, &step))
          model->setGlobalScale(scale);
      }

      ImGui::Spacing();

      // Nodes
      if (ImGui::CollapsingHeader("Nodes"))
      {
        nodeInspector(model);
      }
    }

    ImGui::EndChild();
    ImGui::SetItemDefaultFocus();
    ImGui::End();
  }

  void ModelEditor::nodeInspector(std::shared_ptr<SceneNode> node)
  {
    // Flags. Check if it's selected
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
      ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (auto s = selected_node_.lock())
      if (s->name == node->name)
        flags |= ImGuiTreeNodeFlags_Selected;

    ImGui::PushID(node->name.c_str()); // For Drag N Drop

    // Node tree
    bool open = ImGui::TreeNodeEx(node->name.c_str(), flags);

    //  Allow to drag and drop nodes
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      ImGui::SetDragDropPayload("NODE_TREE", &(node), sizeof(std::shared_ptr<SceneNode>));
      ImGui::Text("Move %s", node->name.c_str());
      ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_TREE"))
      {
        auto dragged_node = *(const std::shared_ptr<SceneNode>*)payload->Data;
        auto parent = dragged_node->getParent();

        if (parent != node.get())
        {
          node->addChild(dragged_node);
          parent->removeChild(dragged_node->name);
        }

        LOG_DEBUG("Dragged %s to %s", dragged_node->name.c_str(), node->name.c_str());
      }
      ImGui::EndDragDropTarget();
    }

    ImGui::PopID(); // For Drag N Drop

    // Select node if clicked
    if (ImGui::IsItemClicked())
    {
      selected_node_ = node;
      editor_->node_editor->setTarget(node->name, node);
    }

    // Open dialog menu if right-clicked
    unsigned int menu_action = 0;
    if (ImGui::BeginPopupContextItem(node->name.c_str()))
    {
      edited_node_ = node;

      if (ImGui::BeginMenu("New node")) // Create a new node
      {
        if (ImGui::MenuItem("Mesh")) menu_action = 1;
        if (ImGui::MenuItem("Material")) menu_action = 2;
        if (ImGui::MenuItem("Camera")) menu_action = 3;
        if (ImGui::MenuItem("Light")) menu_action = 6;
        ImGui::EndMenu();
      }
      if (ImGui::MenuItem("Delete node")) menu_action = 4;
      if (ImGui::MenuItem("Rename")) menu_action = 5;
      ImGui::EndPopup();
    }

    // Inspect children
    if (open)
    {
      for (auto child : *node)
      {
        nodeInspector(child.second);
      }
      ImGui::TreePop();
    }

    if (auto edited = edited_node_.lock())
    {
      // Perform the menu actions
      if (menu_action == 1) ImGui::OpenPopup("New Mesh");
      if (menu_action == 2) ImGui::OpenPopup("New Material");
      if (menu_action == 3) ImGui::OpenPopup("New Camera");
      if (menu_action == 6) ImGui::OpenPopup("New Light");
      if (menu_action == 4) edited->getParent()->removeChild(edited->name);
      if (menu_action == 5) ImGui::OpenPopup("Rename");

      if (ImGui::BeginPopupModal("New Mesh", NULL, ImGuiWindowFlags_AlwaysAutoResize))
      {
        editor_->getInputManager()->enableInput(false);
        static std::string name = "";
        ImGui::InputText("Name", &name);

        static std::string shape = "cube.mesh";
        ImGui::InputText("Shape", &shape); // TODO: Show a selector with all the available files

        // Pick a render pass
        static unsigned int render_pass = model_.lock()->getRenderPass();
        const char* passes[] =
          { "Static", "Actor", "No-Culling", "Sky", "Transparent", "Invisible" };
        if (ImGui::Button("Render Pass"))
        {
          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(passes[render_pass]);
        if (ImGui::BeginPopup("Render Pass Popup"))
        {
          ImGui::Text("Render Pass");
          ImGui::Separator();
          for (int i = 0; i < IM_ARRAYSIZE(passes); i++)
            if (ImGui::Selectable(passes[i]))
              render_pass = i;
          ImGui::EndPopup();
        }

        ImGui::Separator();

        // Submit or cancel the modal
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok", ImVec2(120, 0)) && name != "")
        {
          auto n = std::make_shared<MeshNode>(edited->owner, name);
          n->setRenderPass((RenderPass)render_pass);
          n->setShape(shape);
          edited->addChild(n);

          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }

        ImGui::EndPopup();
      }

      if (ImGui::BeginPopupModal("New Material", NULL, ImGuiWindowFlags_AlwaysAutoResize))
      {
        editor_->getInputManager()->enableInput(false);
        static std::string name = "";
        ImGui::InputText("Name", &name);

        static std::string material = "white.mat.lua";
        ImGui::InputText("Material", &material); // TODO: Show a selector with all the available files

        ImGui::Separator();

        // Submit or cancel the modal
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok", ImVec2(120, 0)) && name != "")
        {
          auto n = std::make_shared<MaterialNode>(edited->owner, name);
          n->setMaterial(material);
          edited->addChild(n);

          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }

        ImGui::EndPopup();
      }

      if (ImGui::BeginPopupModal("New Light", NULL, ImGuiWindowFlags_AlwaysAutoResize))
      {
        editor_->getInputManager()->enableInput(false);
        static std::string name = "";
        ImGui::InputText("Name", &name);

        static std::string shape = "sphere.mesh";
        ImGui::InputText("Shape", &shape); // TODO: Show a selector with all the available files

        static std::string shader = "empty.shader.lua";
        ImGui::InputText("Shader", &shader); // TODO: Show a selector with all the available files

        ImGui::Separator();

        // Submit or cancel the modal
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok", ImVec2(120, 0)) && name != "")
        {
          auto n = std::make_shared<LightNode>(edited->owner, name);
          n->setShape(shape);
          n->setShader(shader);
          edited->addChild(n);

          ImGui::CloseCurrentPopup();
          editor_->getInputManager()->enableInput(true);
        }

        ImGui::EndPopup();
      }
    }
  }

  NodeEditor::NodeEditor(Editor *editor, std::shared_ptr<Scene> scene)
  {
    editor_ = editor;
    scene_ = scene;
  }

  void NodeEditor::setTarget(const std::string &name, std::shared_ptr<SceneNode> node)
  {
    node_name_ = name;
    node_ = node;
  }

  void NodeEditor::draw(bool *active)
  {
    ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
    ImGui::Begin("Node Editor", active, ImGuiWindowFlags_None);

    if (auto node = node_.lock())
    {
      node->drawGuiEditor();
    }

    ImGui::End();
  }

  ModelSaver::ModelSaver(const std::string &file, std::shared_ptr<ActorModel> model)
  {
    file_.open(file.c_str());
    indentation_ = 0;

    if (file_.is_open())
    {
      file_ << "scale = " << model->getGlobalScale() << std::endl;
      file_ << "nodes = {" << std::endl;
      indentation_ += 4;
      for (auto n : *model)
        saveNode(n.second);
      file_ << "}" << std::endl;
    }
  }

  void ModelSaver::saveNode(std::shared_ptr<SceneNode> node)
  {
    file_ << std::string(indentation_, ' ') << "{" << std::endl;
    indentation_ += 4;
    file_ << std::string(indentation_, ' ') << "name = \"" << node->name << "\"," << std::endl;
    file_ << std::string(indentation_, ' ') << "scale = " << std::fixed << std::setprecision(2) << node->getGlobalScale() << "," << std::endl;
    if (node->getOrigin() != glm::vec3(0.f)) saveVec3("origin", node->getOrigin());
    if (node->getPosition() != glm::vec3(0.f)) saveVec3("position", node->getPosition() * 100.f);
    if (node->getEulerOrientation() != glm::vec3(0.f)) saveVec3("rotation", node->getEulerOrientation());

    if (node->getType() == "animated" || node->getType() == "mesh")
    {
      auto mesh = std::static_pointer_cast<MeshNode>(node);
      if (mesh->getScale() != glm::vec3(0.f)) saveVec3("size", mesh->getScale() * 100.f);
      file_ << std::string(indentation_, ' ') << "shape = \"" << mesh->getShape() << "\"," << std::endl;
      saveVec2("uv_map", mesh->getUvMap());
      if (mesh->getTextureSize() != mesh->getScale()) saveVec3("texture_size", mesh->getTextureSize() * 100.f);
      if (mesh->getColorPrimary() != glm::tvec3<unsigned char>(255)) saveVec3("color_primary", mesh->getColorPrimary());
      if (mesh->getColorSecondary() != glm::tvec3<unsigned char>(255)) saveVec3("color_secondary", mesh->getColorSecondary());
      if (mesh->getColorDetail() != glm::tvec3<unsigned char>(255)) saveVec3("color_detail", mesh->getColorDetail());
      if (mesh->getColorDetail2() != glm::tvec3<unsigned char>(255)) saveVec3("color_detail2", mesh->getColorDetail2());
      if ((mesh->getType() == "animated" && mesh->getNewNodeType() == "") || mesh->getNewNodeType() == "animated")
        file_ << std::string(indentation_, ' ') << "animated = true," << std::endl;
    }
    else if (node->getType() == "camera")
    {
      file_ << std::string(indentation_, ' ') << "type = \"camera\"" << std::endl;
    }
    else if (node->getType() == "material")
    {
      file_ << std::string(indentation_, ' ') << "type = \"material\"" << std::endl;
    }

    if (node->size() > 0)
    {
      file_ << std::string(indentation_, ' ') << "nodes = {" << std::endl;
      indentation_ += 4;
      for (auto n : *node)
        saveNode(n.second);
      indentation_ -= 4;
      file_ << std::string(indentation_, ' ') << "}" << std::endl;
    }
    indentation_ -= 4;
    file_ << std::string(indentation_, ' ') << "}," << std::endl;
  }

  void ModelSaver::saveVec3(const std::string &name, const glm::vec3 &vector)
  {
    file_ << std::string(indentation_, ' ') << name << " = "
          << "{" << std::fixed << std::setprecision(2) << vector.x << ", "
          << std::fixed << std::setprecision(2) << vector.y << ", "
          << std::fixed << std::setprecision(2) << vector.z << "}," << std::endl;
  }

  void ModelSaver::saveVec2(const std::string &name, const glm::vec2 &vector)
  {
    file_ << std::string(indentation_, ' ') << name << " = "
          << "{" << std::fixed << std::setprecision(2) << vector.x << ", "
          << std::fixed << std::setprecision(2) << "}," << std::endl;
  }

  void ModelSaver::saveVec3(const std::string &name, const glm::tvec3<unsigned char> &vector)
  {
    file_ << std::string(indentation_, ' ') << name << " = "
          << "{" << (unsigned int)vector.x << ", " << (unsigned int)vector.y << ", "
          << (unsigned int)vector.z << "}," << std::endl;
  }

  void ModelSaver::saveVec2(const std::string &name, const glm::tvec2<unsigned short> &vector)
  {
    file_ << std::string(indentation_, ' ') << name << " = "
          << "{" << vector.x << ", " << vector.y << "}," << std::endl;
  }
}
