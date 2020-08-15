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

#include "ActorModel.hpp"
#include "Scene.hpp"

namespace Tarbora {
  ActorModel::ActorModel(
    const ActorId &id, RenderPass render_pass,
    const std::string &model, const std::string &material
  )
    : MaterialNode(id, id)
  {
    setMaterial(material);
    model_ = this;
    model_name_ = model;
    setRenderPass(render_pass);
    ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
    LuaTable actor = resource->get("actor");

    if (actor.valid())
      load(actor, &nodes_);
  }

  ActorModel::~ActorModel()
  {
    nodes_.clear();
  }

  void ActorModel::load(const LuaTable &table, NodeMap *map)
  {
    addColor(NamedColor("White", {255, 255, 255}));
    for (auto color : table.get("colors", true))
      addColor(
        NamedColor(color.first.getAs<std::string>(), color.second.getAs<glm::vec3>())
      );

    SceneNode::load(table, map);
    setGlobalScale(table.get<float>("scale", 1.f, true));
  }

  void ActorModel::drawGuiEditor()
  {
    MaterialNode::drawGuiEditor();
    ImGui::Spacing();
    drawColorList();
  }

  void ActorModel::drawColorList()
  {
    if (ImGui::CollapsingHeader("Colors"))
    {
      ImGui::Spacing();

      unsigned int i = 0;
      for (auto color = colors_.begin(); color != colors_.end();)
      {
        if ((*color)->first != "White")
        {
          std::string label = "##" + std::to_string(i);

          ImGui::InputText(label.c_str(), &(*color)->first);
          ImGui::SameLine();

          glm::vec3 c = (*color)->second;
          c /= 255.f;
          float col[4] = {c.x, c.y, c.z, 1.f};
          ImGuiColorEditFlags flags =
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;
          if (ImGui::ColorEdit3(label.c_str(), col, flags))
            (*color)->second = glm::make_vec3(col) * 255.f;

          ImGui::SameLine();
          if (ImGui::Button((ICON_FK_TRASH_O + label).c_str()))
            color = colors_.erase(color);
          else
            ++color;

          i++;
        }
        else{
          ++color;
        }
      }

      if (ImGui::Button("New color"))
      {
        addColor({"Color " + std::to_string(i), Color(255)});
      }

      ImGui::Spacing();
    }
  }

  void ActorModel::write(LuaFile *file)
  {
    file->beginGlobalTable("actor");
    file->write("scale", getGlobalScale());
    if (colors_.size() > 1)
    {
      file->beginTable("colors");
      for (auto color : colors_)
      {
        if (color->first != "White")
          file->write("[\"" + color->first + "\"]", glm::vec3(color->second));
      }
      file->closeTable();
    }
    writeChildren(file);
    file->closeGlobalTable();
  }

  void ActorModel::update(Scene *scene, float delta_time)
  {
    if (animation_controller_)
    {
      animation_controller_->update(delta_time);
    }

    MaterialNode::update(scene, delta_time);
  }

  SceneNodePtr ActorModel::getChild(const std::string &name)
  {
    auto itr = nodes_.find(name);
    if (itr != nodes_.end())
    {
      return itr->second;
    }
    return std::shared_ptr<SceneNode>();
  }

  void ActorModel::startAnimation(Animation animation, bool background)
  {
    if (!animation_controller_)
    {
      animation_controller_ = std::make_unique<AnimationController>(this);
    }
    animation_controller_->startAnimation(animation, background);
  }

  void ActorModel::endAnimation(
    const std::string &name, StopMode mode, float fade_out_timer
  )
  {
    if (animation_controller_)
    {
      animation_controller_->endAnimation(name, mode, fade_out_timer);
    }
  }

  void ActorModel::addColor(const NamedColor &color)
  {
    colors_.emplace_back(std::make_shared<NamedColor>(color));
  }

  std::weak_ptr<NamedColor> ActorModel::getColor(const std::string &color_name)
  {
    auto color = std::find_if(colors_.begin(), colors_.end(), [&color_name](auto color) {
      return color->first == color_name;
    });
    if (color == colors_.end())
    {
      color = std::find_if(colors_.begin(), colors_.end(), [](auto color) {
        return color->first == "White";
      });
      if (color == colors_.end())
        LOG_ERR("Bad color specification");
    }
    return *color;
  }

  Colors* ActorModel::getColors()
  {
    return &colors_;
  }
}
