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

#include "SceneNode.hpp"
#include "Scene.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
  SceneNode::SceneNode(const SceneNode &other)
  {
    owner = other.owner;
    name = other.name;
    origin_ = other.origin_;
    position_ = other.position_;
    orientation_ = other.orientation_;
    scale_ = other.scale_;
    global_scale_ = other.global_scale_;

    for (auto child : other.children_)
      addChild(child.second->clone());
  }

  SceneNode::~SceneNode()
  {
    children_.clear();
  }

  void SceneNode::update(Scene *scene, float delta_time)
  {
    scene_ = scene;

    for (auto child : children_)
      child.second->update(scene, delta_time);
  }

  void SceneNode::drawChildren(Scene *scene)
  {
    if (dirty_)
      fixDirty();

    draw(scene);

    for (auto child : children_)
    {
      child.second->drawChildren(scene);
    }

    afterDraw(scene);
  }

  void SceneNode::drawGuiEditor()
  {
    guiName();
    ImGui::Spacing();
    guiTransformFixedSize();
  }

  void SceneNode::guiName()
  {
    ImGui::InputText("Name", &name);
  }

  void SceneNode::guiTransform(bool fixedSize)
  {
    static short current_guizmo_mode_ = 0;
    static ImGuizmo::OPERATION current_guizmo_op_(ImGuizmo::TRANSLATE);

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
      ImGuizmo::Enable(true);

      ImGui::Spacing();
      if (ImGui::RadioButton("Local", current_guizmo_mode_ == 0))
        current_guizmo_mode_ = 0;

      if (!fixedSize && current_guizmo_op_ == ImGuizmo::SCALE)
        current_guizmo_mode_ = 0;
      else
      {
        //ImGui::SameLine();
        //if (ImGui::RadioButton("Parent", current_guizmo_mode_ == 1))
        //  current_guizmo_mode_ = 1;

        ImGui::SameLine();
        if (ImGui::RadioButton("World", current_guizmo_mode_ == 2))
          current_guizmo_mode_ = 2;
      }

      if (ImGui::RadioButton("Move", current_guizmo_op_ == ImGuizmo::TRANSLATE))
        current_guizmo_op_ = ImGuizmo::TRANSLATE;

      ImGui::SameLine();
      if (ImGui::RadioButton("Rotate", current_guizmo_op_ == ImGuizmo::ROTATE))
        current_guizmo_op_ = ImGuizmo::ROTATE;

      if (!fixedSize)
      {
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", current_guizmo_op_ == ImGuizmo::SCALE))
          current_guizmo_op_ = ImGuizmo::SCALE;
      }

      ImGui::Spacing();
      glm::vec3 p = getPosition()*100.f;
      float position[3] = {p.x, p.y, p.z};
      if (ImGui::DragFloat3("Position", position, 1.f, 0.f, 0.f, "%.2f cm"))
        setPosition(glm::make_vec3(position)/100.f);

      ImGui::Spacing();
      glm::vec3 r = getEulerOrientation();
      float rotation[3] = {r.x, r.y, r.z};
      if (ImGui::DragFloat3("Orientation", rotation, 1.f, -180.f, 180.f, "%.2f°"))
        setOrientation(glm::make_vec3(rotation));

      if (!fixedSize)
      {
        ImGui::Spacing();
        glm::vec3 s = getScale()*100.f;
        float scale[3] = {s.x, s.y, s.z};
        if (ImGui::DragFloat3("Scale", scale, 1.f, 1.f, 1000.f, "%.2f cm"))
          setScale(glm::make_vec3(scale)/100.f);
      }

      ImGui::Spacing();

      if (!fixedSize)
      {
        ImGui::Spacing();
        float g_scale = getGlobalScale();
        if (ImGui::DragFloat("Global Scale", &g_scale, 0.1f, 0.1f, 1000.f, "%.2f"))
          setGlobalScale(g_scale);
      }

      ImGui::Spacing();
      glm::vec3 o = getOrigin();
      float origin[3] = {o.x, o.y, o.z};
      if (ImGui::DragFloat3("Origin", origin, 0.01f, -0.5f, 0.5f, "%.2f"))
        setOrigin(glm::make_vec3(origin));

      ImGui::Spacing();

      fixDirty();

      glm::mat4 t;

      if (current_guizmo_mode_ == 1)
      {
        t = (current_guizmo_op_ == ImGuizmo::SCALE) ?
          parent_->world_ : glm::translate(parent_->world_, getOrigin());
        t = glm::translate(t, getPosition());
      }
      else
      {
        t = (current_guizmo_op_ == ImGuizmo::SCALE) ?
          transform_ : glm::translate(transform_, getOrigin());
      }

      float transform[] = {
        t[0][0], t[0][1], t[0][2], t[0][3],
        t[1][0], t[1][1], t[1][2], t[1][3],
        t[2][0], t[2][1], t[2][2], t[2][3],
        t[3][0], t[3][1], t[3][2], t[3][3]
      };

      if (scene_)
      {
        ImGuizmo::Manipulate(
          glm::value_ptr(scene_->getView()), glm::value_ptr(scene_->getProjection()),
          current_guizmo_op_,
          current_guizmo_mode_ == 2 ? ImGuizmo::WORLD : ImGuizmo::LOCAL,
          transform
        );
      }

      float pos[3], rot[3], sca[3];
      glm::vec3 gpos, grot, gsca;

      if (current_guizmo_op_ == ImGuizmo::SCALE)
      {
        ImGuizmo::DecomposeMatrixToComponents(transform, pos, rot, sca);
        gsca = glm::make_vec3(sca);

        if (glm::abs(glm::length(gsca - getScale())) > 0.001f)
          setScale(gsca);
      }
      else
      {
        glm::vec3 origin = getRawOrigin();
        t = glm::make_mat4(transform);
        if (current_guizmo_mode_ == 1)
        {
          t = glm::translate(t, -getPosition());
          t = t * local_;
          //origin = glm::vec3(0.f);
        }
        t = glm::inverse(parent_->world_) * t;

        float transform2[] = {
          t[0][0], t[0][1], t[0][2], t[0][3],
          t[1][0], t[1][1], t[1][2], t[1][3],
          t[2][0], t[2][1], t[2][2], t[2][3],
          t[3][0], t[3][1], t[3][2], t[3][3]
        };

        ImGuizmo::DecomposeMatrixToComponents(transform2, pos, rot, sca);
        gpos = glm::make_vec3(pos) - origin;
        grot = glm::make_vec3(rot);

        if (glm::abs(glm::length(gpos - getPosition())) > 0.001f)
          setPosition(gpos);

        if (glm::abs(glm::length(grot - getEulerOrientation())) > 0.1f)
          setOrientation(grot);
      }
    }
  }

  void SceneNode::load(const LuaTable &table, NodeMap *map)
  {
    RenderPass pass = (RenderPass)table.get<unsigned int>("render_pass", 0, true);
    if (pass != RenderPass::Null)
    {
      setRenderPass(pass);
    }
    else if (parent_)
    {
      setRenderPass(parent_->render_pass_);
    }

    setGlobalScale(table.get<float>("scale", 1.f, true));
    setOrigin(table.get<glm::vec3>("origin", true));
    setPosition(table.get<glm::vec3>("position", true)/100.f);
    setOrientation(table.get<glm::vec3>("rotation", true));
   
    setScale(table.get<glm::vec3>("size", true)/100.f);

    for (auto node : table.get("nodes", true))
    {
      LuaTable t = node.second.getAs<LuaTable>();
      std::string n = t.get<std::string>("name");
      std::string type = t.get<std::string>("type", "mesh", true);
      std::shared_ptr<SceneNode> new_node;

      // TODO: Automate this
      if (type == "mesh")
      {
        if (t.get<bool>("animated", false, true))
        {
          new_node = addChild(std::make_shared<AnimatedNode>(owner, n));
        }
        else
        {
          new_node = addChild(std::make_shared<MeshNode>(owner, n));
        }
      }
      else if (type == "camera")
      {
        LOG_DEBUG("Camera %s %s", n.c_str(), owner.c_str());
        new_node = addChild(std::make_shared<Camera>(owner, n));
      }
      else if (type == "material")
      {
        new_node = addChild(std::make_shared<MaterialNode>(owner, n));
      }
      else if (type == "light")
      {
        new_node = addChild(std::make_shared<LightNode>(owner, n));
      }

      new_node->load(t, map);

      if (map)
      {
        map->emplace(n, new_node);
      }
    }
  }

  void SceneNode::write(LuaFile *file)
  {
    writeName(file);
    writeTransformFixedSize(file);
    writeChildren(file);
  }

  void SceneNode::writeName(LuaFile *file)
  {
    file->write("name", name);
    if (getType() != "mesh")
    {
      if (getType() == "animated")
        file->write("animated", true);
      else
        file->write("type", getType());
    }
    if (parent_ && getRenderPass() != parent_->getRenderPass())
      file->write("render_pass", getRenderPass());
  }

  void SceneNode::writeTransform(LuaFile *file, bool fixedSize)
  {
    if (getPosition() != glm::vec3(0.f))
      file->write("position", getPosition() * 100.f);
    if (getOrientation() != glm::quat(glm::vec3(0.f)))
      file->write("rotation", getEulerOrientation());
    if (!fixedSize && getScale() != glm::vec3(1.f))
      file->write("size", getScale() * 100.f);
    if (!fixedSize && getGlobalScale() != 1.f)
      file->write("global_scale", getGlobalScale());
    if (getOrigin() != glm::vec3(0.f))
      file->write("origin", getOrigin());
  }

  void SceneNode::writeChildren(LuaFile *file)
  {
    if (!children_.empty())
    {
      file->beginTable("nodes");
      for (auto child : children_)
      {
        file->beginTable();
        child.second->write(file);
        file->closeTable();
      }
      file->closeTable();
    }
  }

  SceneNodePtr SceneNode::addChild(SceneNodePtr child)
  {
    children_[child->name] = child;
    child->parent_ = this;
    if (child->model_ == nullptr) child->model_ = model_;
    child->setDirty();
    return child;
  }

  SceneNodePtr SceneNode::getChild(const std::string &name)
  {
    auto itr = children_.find(name);
    if (itr != children_.end())
    {
      return itr->second;
    }
    return SceneNodePtr();
  }

  bool SceneNode::removeChild(const std::string &name)
  {
    auto child = children_.find(name);
    if (child != children_.end())
    {
      children_.erase(child);
      return true;
    }
    return false;
  }

  void SceneNode::setRenderPass(RenderPass pass)
  {
    render_pass_ = pass;
  }

  RenderPass SceneNode::getRenderPass()
  {
    return render_pass_;
  }

  void SceneNode::setPosition(const glm::vec3 &position)
  {
    position_ = position;
    setVeryDirty();
  }

  void SceneNode::setOrientation(const glm::vec3 &orientation)
  {
    setOrientation(glm::quat(glm::radians(orientation)));
  }

  void SceneNode::setOrientation(const glm::quat &orientation)
  {
    orientation_ = orientation;
    setVeryDirty();
  }

  void SceneNode::setScale(const glm::vec3 &scale)
  {
    if (scale.x > 0.f && scale.y > 0.f && scale.z > 0.f)
    {
      origin_ *= scale/scale_;
      scale_ = scale;
      setVeryDirty();
    }
  }

  void SceneNode::setGlobalScale(float scale)
  {
    if (scale > 0.f)
    {
      origin_ *= scale/global_scale_;
      global_scale_ = scale;
      setVeryDirty();
    }
  }

  void SceneNode::setOrigin(const glm::vec3 &origin)
  {
    origin_ = origin * global_scale_ * scale_;
    setVeryDirty();
  }

  const glm::vec3& SceneNode::getPosition()
  {
    return position_;
  }

  glm::vec3 SceneNode::getEulerOrientation()
  {
    return glm::degrees(glm::eulerAngles(orientation_));
  }

  const glm::quat& SceneNode::getOrientation()
  {
    return orientation_;
  }

  const glm::vec3& SceneNode::getScale()
  {
    return scale_;
  }

  float SceneNode::getGlobalScale()
  {
    return global_scale_;
  }

  glm::vec3 SceneNode::getOrigin()
  {
    return origin_ / global_scale_ / scale_;
  }

  glm::vec3 SceneNode::getRawOrigin()
  {
    return origin_;
  }

  void SceneNode::setLocal(const TransformProps &props)
  {
    position_ = props.position;
    orientation_ = props.orientation;
    if (glm::length(props.scale) > 0.f)
      scale_ = props.scale;
    if (props.global_scale > 0.f)
      global_scale_ = props.global_scale;

    setVeryDirty();
  }

  TransformProps SceneNode::getLocalDecomposed()
  {
    TransformProps props;
    props.position = position_;
    props.orientation = orientation_;
    props.scale = scale_;
    props.global_scale = global_scale_;

    return props;
  }

  const glm::mat4& SceneNode::getLocal()
  {
    if (very_dirty_)
      fixDirty();
    return local_;
  }

  const glm::mat4& SceneNode::getWorld()
  {
    if (dirty_)
      fixDirty();
    return world_;
  }

  void SceneNode::setDirty()
  {
    if (!dirty_)
    {
      dirty_ = true;
      for (auto child : children_)
        child.second->setDirty();
    }
  }

  void SceneNode::setVeryDirty()
  {
    very_dirty_ = true;
    setDirty();
  }

  void SceneNode::fixDirty()
  {
    if (very_dirty_)
      fixLocal();

    fixWorld();
    fixTransform();

    dirty_ = false;
  }

  void SceneNode::fixLocal()
  {
    local_ = glm::translate(glm::mat4(1.f), position_ + origin_);
    local_ = local_ * glm::mat4_cast(orientation_);
    local_ = glm::translate(local_, -origin_);
    local_ = glm::scale(local_, glm::vec3(global_scale_));
    very_dirty_ = false;
  }

  void SceneNode::fixWorld()
  {
    if (parent_)
      world_ = parent_->getWorld() * local_;
    else
      world_ = local_;
  }

  void SceneNode::fixTransform()
  {
    transform_ = glm::scale(world_, scale_);
  }

  const glm::mat4& Camera::getView()
  {
    if (dirty_)
      fixDirty();
    return view_;
  }

  void Camera::fixDirty()
  {
    SceneNode::fixDirty();
    fixView();
  }

  void Camera::fixView()
  {
    glm::vec3 front = glm::vec3(world_[2]);
    glm::vec3 position = glm::vec3(world_[3]);
    view_ = glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
  }

  MaterialNode::MaterialNode(const MaterialNode &other) : SceneNode(other)
  {
    material_ = other.material_;
    material_name_ = other.material_name_;
  }

  void MaterialNode::draw(Scene *scene)
  {
    if (material_ != nullptr)
      scene->getRenderQueue()->pushMaterial(material_);
  }

  void MaterialNode::drawGuiEditor()
  {
    guiName();
    guiProperties();
    ImGui::Spacing();
    guiTransformFixedSize();
  }

  void MaterialNode::guiProperties()
  {
    ImGui::Spacing();
    std::string material = getMaterial();
    auto pos = material.find(".");
    material = material.substr(0, pos);
    if (ImGui::InputText("Material", &material))
      setMaterial(material + ".mat.lua");

    ImGui::Spacing();
  }

  void MaterialNode::afterDraw(Scene *scene)
  {
    if (material_ != nullptr)
      scene->getRenderQueue()->popMaterial();
  }

  void MaterialNode::load(const LuaTable &table, NodeMap *map)
  {
    SceneNode::load(table, map);
    setMaterial(table.get<std::string>("material"));
  }

  void MaterialNode::write(LuaFile *file)
  {
    writeName(file);
    writeProperties(file);
    writeTransformFixedSize(file);
    writeChildren(file);
  }

  void MaterialNode::writeProperties(LuaFile *file)
  {
    file->write("material", getMaterial());
  }

  void MaterialNode::setMaterial(const std::string &name)
  {
    material_ = ResourcePtr<Material>(
      "materials/" + name,
      "materials/missing.mat.lua");
    material_name_ = name;
  }

  MeshNode::MeshNode(const MeshNode &other) : SceneNode(other)
  {
    mesh_ = other.mesh_;
    mesh_name_ = other.mesh_name_;
    uv_map_ = other.uv_map_;
    mesh_size_ = other.mesh_size_;
    texture_size_ = other.texture_size_;
    auto_texture_size_ = other.auto_texture_size_;
    for (unsigned int i = 0; i < 4; i++)
      color_[i] = other.color_[i];
  }

  void MeshNode::draw(Scene *scene)
  {
    if (dirty_)
      fixDirty();

    if (mesh_ != nullptr)
    {
      scene->getRenderQueue()->drawMesh(
        render_pass_,
        mesh_,
        transform_,
        uv_map_,
        auto_texture_size_ ? scale_ : mesh_size_,
        auto_texture_size_ ? scale_ : texture_size_,
        getColor(0)->second,
        getColor(1)->second,
        getColor(2)->second,
        getColor(3)->second
      );
    }
  }

  void MeshNode::drawGuiEditor()
  {
    guiName();
    guiProperties();
    ImGui::Spacing();
    guiTransform();
    ImGui::Spacing();
    guiColors();
    ImGui::Spacing();
    guiTexture();
  }

  void MeshNode::guiProperties()
  {
    ImGui::Spacing();
    std::string shape = getShape();
    auto pos = shape.find(".");
    shape = shape.substr(0, pos);
    if (ImGui::InputText("Shape", &shape))
      setShape(shape + ".mesh");

    ImGui::Spacing();
    const char* passes[] = {
      "Null", "Static", "Actor", "No-Culling", "Sky", "Transparent", "Invisible"
    };
    int render_pass = getRenderPass();
    if (ImGui::BeginCombo("Render Pass", passes[render_pass]))
    {
      for (int i = 1; i < IM_ARRAYSIZE(passes); i++)
      {
        bool is_selected = (render_pass == i);
        if (ImGui::Selectable(passes[i], is_selected))
        {
          render_pass = i;
          setRenderPass((RenderPass)i);
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    ImGui::Spacing();
    bool animated = (getType() == "animated" && new_type_ == "") ||
      new_type_ == "animated";
    if (ImGui::Checkbox("Animated", &animated))
    {
      new_type_ = animated ? "animated" : "mesh";
    }

    ImGui::Spacing();
  }

  void MeshNode::guiColors()
  {
    if (ImGui::CollapsingHeader("Colors"))
    {
      ImGui::Spacing();
      auto c1 = getColor(0);
      if (ImGui::BeginCombo("Color Primary", c1->first.c_str()))
      {
        for (auto c : *(model_->getColors()))
        {
          bool is_selected = (c->first == c1->first);
          if (ImGui::Selectable(c->first.c_str(), is_selected))
            setColor(0, c->first);
        }
        ImGui::EndCombo();
      }

      ImGui::Spacing();
      auto c2 = getColor(1);
      if (ImGui::BeginCombo("Color Secondary", c2->first.c_str()))
      {
        for (auto c : *(model_->getColors()))
        {
          bool is_selected = (c->first == c2->first);
          if (ImGui::Selectable(c->first.c_str(), is_selected))
            setColor(1, c->first);
        }
        ImGui::EndCombo();
      }

      ImGui::Spacing();
      auto c3 = getColor(2);
      if (ImGui::BeginCombo("Color Detail", c3->first.c_str()))
      {
        for (auto c : *(model_->getColors()))
        {
          bool is_selected = (c->first == c3->first);
          if (ImGui::Selectable(c->first.c_str(), is_selected))
            setColor(2, c->first);
        }
        ImGui::EndCombo();
      }

      ImGui::Spacing();
      auto c4 = getColor(3);
      if (ImGui::BeginCombo("Color Emissive", c4->first.c_str()))
      {
        for (auto c : *(model_->getColors()))
        {
          bool is_selected = (c->first == c4->first);
          if (ImGui::Selectable(c->first.c_str(), is_selected))
            setColor(3, c->first);
        }
        ImGui::EndCombo();
      }

      ImGui::Spacing();
    }
  }

  void MeshNode::guiTexture()
  {
    if (ImGui::CollapsingHeader("Texture"))
    {
      ImGui::Spacing();
      glm::tvec2<int> uv = getUvMap();
      int uv_map[2] = {uv.x, uv.y};
      if (ImGui::DragInt2("UV Map", uv_map))
        setUvMap(glm::make_vec2(uv_map));

      ImGui::Spacing();
      ImGui::Checkbox("Auto Texture Size", &auto_texture_size_);

      if (!auto_texture_size_)
      {
        ImGui::Spacing();
        glm::vec3 ts = getTextureSize()*100.f;
        float texture_size[3] = {ts.x, ts.y, ts.z};
        if (ImGui::DragFloat3("Texture Size", texture_size, 1.f, 1.f, 1000.f, "%.2f cm"))
          setTextureSize(glm::make_vec3(texture_size)/100.f);

        ImGui::Spacing();
        glm::vec3 ms = getMeshSize()*100.f;
        float mesh_size[3] = {ms.x, ms.y, ms.z};
        if (ImGui::DragFloat3("Mesh Size", mesh_size, 1.f, 1.f, 1000.f, "%.2f cm"))
          setMeshSize(glm::make_vec3(mesh_size)/100.f);
      }

      ImGui::Spacing();
    }
  }

  void MeshNode::load(const LuaTable &table, NodeMap *map)
  {
    SceneNode::load(table, map);

    setShape(table.get<std::string>("shape"));

    glm::vec3 mesh_size = table.get<glm::vec3>("mesh_size", true)/100.f;
    if (mesh_size == glm::vec3(0.f))
      mesh_size = scale_;
    setMeshSize(mesh_size);

    glm::vec3 texture_size = table.get<glm::vec3>("texture_size", true)/100.f;
    if (texture_size == glm::vec3(0.f))
      texture_size = scale_;
    setTextureSize(texture_size);

    setUvMap(table.get<glm::vec2>("uv_map", true));
    setColor(0, table.get<std::string>("color_primary", "White", true));
    setColor(1, table.get<std::string>("color_secondary", "White", true));
    setColor(2, table.get<std::string>("color_detail", "White", true));
    setColor(3, table.get<std::string>("color_emissive", "White", true));
  }

  void MeshNode::write(LuaFile *file)
  {
    writeName(file);
    writeTransform(file);
    writeMesh(file);
    writeChildren(file);
  }

  void MeshNode::writeMesh(LuaFile *file)
  {
    file->write("shape", getShape());
    file->write("uv_map", glm::vec2(getUvMap()));
    if (getTextureSize() != getScale())
      file->write("texture_size", getTextureSize() * 100.f);
    if (getMeshSize() != getScale())
      file->write("mesh_size", getMeshSize() * 100.f);
    if (getColor(0)->first != "White")
      file->write("color_primary", getColor(0)->first);
    if (getColor(1)->first != "White")
      file->write("color_secondary", getColor(1)->first);
    if (getColor(2)->first != "White")
      file->write("color_detail", getColor(2)->first);
    if (getColor(3)->first != "White")
      file->write("color_emissive", getColor(3)->first);
  }

  void MeshNode::setShape(const std::string &mesh)
  {
    mesh_ = ResourcePtr<Mesh>("meshes/" + mesh, "meshes/cube.mesh");
    mesh_name_ = mesh;
  }

  void MeshNode::setUvMap(const glm::tvec2<unsigned short> &uv)
  {
    uv_map_ = uv;
  }

  void MeshNode::setMeshSize(const glm::vec3 &size)
  {
    mesh_size_ = size;
    auto_texture_size_ = (mesh_size_ == scale_ && texture_size_ == scale_);
  }

  void MeshNode::setTextureSize(const glm::vec3 &size)
  {
    texture_size_ = size;
    auto_texture_size_ = (texture_size_ == scale_ && mesh_size_ == scale_);
  }

  void MeshNode::setColor(unsigned int i, const std::string &color)
  {
    color_[i] = model_->getColor(color);
  }

  const std::string& MeshNode::getShape()
  {
    return mesh_name_;
  }

  const glm::tvec2<unsigned short>& MeshNode::getUvMap()
  {
    return uv_map_;
  }

  const glm::vec3& MeshNode::getMeshSize()
  {
    return mesh_size_;
  }

  const glm::vec3& MeshNode::getTextureSize()
  {
    return texture_size_;
  }

  bool MeshNode::getAutoTextureSize()
  {
    return auto_texture_size_;
  }

  const std::shared_ptr<NamedColor> MeshNode::getColor(unsigned int i)
  {
    if (auto color = color_[i].lock())
      return color;
    else
    {
      setColor(i, "White");
      return getColor(i);
    }
  }

  void AnimatedNode::resetAll()
  {
    position_anim_ = glm::vec3(0.f);
    orientation_anim_ = glm::quat({0.f, 0.f, 0.f});
    scale_anim_ = glm::vec3(0.f);
    global_scale_anim_ = 1.f;
    uv_map_anim_ = glm::tvec2<unsigned short>(0);
    for (unsigned int i = 0; i < 4; i++)
      color_anim_[i] = Color(0);
  }

  void AnimatedNode::draw(Scene *scene)
  {
    if (dirty_)
      fixDirty();
   
    if (mesh_ != nullptr)
    {
      scene->getRenderQueue()->drawMesh(
        render_pass_,
        mesh_,
        transform_,
        uv_map_ + uv_map_anim_,
        auto_texture_size_ ? scale_ : mesh_size_,
        auto_texture_size_ ? scale_ : texture_size_,
        getColor(0)->second + color_anim_[0],
        getColor(1)->second + color_anim_[1],
        getColor(2)->second + color_anim_[2],
        getColor(3)->second + color_anim_[3]
      );
    }

    resetAll();
  }

  void AnimatedNode::setPositionAnimation(const glm::vec3 &position)
  {
    position_anim_ = position;
    setVeryDirty();
  }

  void AnimatedNode::setOrientationAnimation(const glm::vec3 &orientation)
  {
    orientation_anim_ = glm::quat(glm::radians(orientation));
    setVeryDirty();
  }

  void AnimatedNode::setOrientationAnimation(const glm::quat &orientation)
  {
    orientation_anim_ = orientation;
    setVeryDirty();
  }

  void AnimatedNode::setScaleAnimation(const glm::vec3 &scale)
  {
    scale_anim_ = scale;
    setDirty();
  }

  void AnimatedNode::setGlobalScaleAnimation(float scale)
  {
    global_scale_anim_ = scale;
    setVeryDirty();
  }

  void AnimatedNode::setUvMapAnimation(const glm::tvec2<unsigned short> &uv)
  {
    uv_map_anim_ = uv;
  }

  void AnimatedNode::setColorAnimation(unsigned int i, const Color &color)
  {
    if (i < 4)
      color_anim_[i] = color;
  }

  const glm::vec3& AnimatedNode::getPositionAnimation()
  {
    return position_anim_;
  }

  const glm::quat& AnimatedNode::getOrientationAnimation()
  {
    return orientation_anim_;
  }

  const glm::vec3& AnimatedNode::getScaleAnimation()
  {
    return scale_anim_;
  }

  float AnimatedNode::getGlobalScaleAnimation()
  {
    return global_scale_anim_;
  }

  const glm::tvec2<unsigned short>& AnimatedNode::getUvMapAnimation()
  {
    return uv_map_anim_;
  }

  const glm::tvec3<unsigned char>& AnimatedNode::getColorAnimation(unsigned int i)
  {
    return (i < 4) ? color_anim_[i] : color_anim_[0];
  }

  void AnimatedNode::fixLocal()
  {
    local_ = glm::translate(glm::mat4(1.f), position_ + position_anim_ + origin_);
    local_ = local_ * glm::mat4_cast(orientation_ * orientation_anim_);
    local_ = glm::translate(local_, -origin_);
    local_ = glm::scale(local_, glm::vec3(global_scale_ * global_scale_anim_));
    very_dirty_ = false;
  }

  void AnimatedNode::fixTransform()
  {
    transform_ = glm::scale(world_, scale_ + scale_anim_);
  }

  LightNode::LightNode(const LightNode &other) : SceneNode(other)
  {
    mesh_ = other.mesh_;
    mesh_name_ = other.mesh_name_;
    shader_ = other.shader_;
    shader_name_ = other.shader_name_;
    ambient_ = other.ambient_;
    diffuse_ = other.diffuse_;
    specular_ = other.specular_;
    direction_ = other.direction_;
    l_att_ = other.l_att_;
    q_att_ = other.q_att_;
  }

  void LightNode::draw(Scene *scene)
  {
    if (dirty_)
      fixDirty();

    if (mesh_ != nullptr)
    {
      scene->getRenderQueue()->drawLight(
        mesh_,
        shader_,
        transform_,
        ambient_,
        diffuse_,
        specular_,
        direction_,
        intensity_,
        l_att_,
        q_att_
      );
    }
  }

  void LightNode::load(const LuaTable &table, NodeMap *map)
  {
    SceneNode::load(table, map);

    setShape(table.get<std::string>("shape"));
    setShader(table.get<std::string>("shader"));

    setAmbient(table.get<glm::vec3>("ambient", glm::vec3(1.f), true)/255.f);
    setDiffuse(table.get<glm::vec3>("diffuse", glm::vec3(1.f), true)/255.f);
    setSpecular(table.get<glm::vec3>("specular", glm::vec3(1.f), true)/255.f);

    setDirection(table.get<glm::vec3>("direction", glm::vec3(1.f), true));

    setIntensity(table.get<float>("intensity", 1.f, true));
    setAttenuation(table.get<glm::vec2>("attenuation", glm::vec2(1.f), true));
  }

  void LightNode::write(LuaFile *file)
  {
    writeName(file);
    writeTransformFixedSize(file);
    writeLight(file);
  }

  void LightNode::writeLight(LuaFile *file)
  {
    file->write("shape", getShape());
    file->write("shader", getShader());
    file->write("ambient", getAmbient()*255.f);
    file->write("diffuse", getDiffuse()*255.f);
    file->write("specular", getSpecular()*255.f);
    file->write("direction", getDirection());
    file->write("intensity", getIntensity());
    file->write("attenuation", getAttenuation());
  }

  void LightNode::drawGuiEditor()
  {
    guiName();
    guiProperties();
    ImGui::Spacing();
    guiTransformFixedSize();
    ImGui::Spacing();
    guiLight();
  }

  void LightNode::guiProperties()
  {
    ImGui::Spacing();
    std::string shape = getShape();
    auto pos = shape.find(".");
    shape = shape.substr(0, pos);
    if (ImGui::InputText("Shape", &shape))
      setShape(shape + ".mesh");

    ImGui::Spacing();
    std::string shader = getShader();
    pos = shader.find(".");
    shader = shader.substr(0, pos);
    if (ImGui::InputText("Shader", &shader))
      setShader(shader + ".shader.lua");

    ImGui::Spacing();
  }

  void LightNode::guiLight()
  {
    if (ImGui::CollapsingHeader("Light"))
    {
      ImGui::Spacing();
      glm::vec3 d = getDirection();
      float dir[3] = {d.x, d.y, d.z};
      if (ImGui::DragFloat3("Direction", dir, .1f))
        setDirection(glm::make_vec3(dir));

      ImGui::Spacing();
      float i = getIntensity();
      if (ImGui::DragFloat("Intensity", &i, .01f, .001f, 10.f))
        setIntensity(i);
     
      glm::vec2 a = getAttenuation();
      float att[2] = {a.x, a.y};
      if (ImGui::DragFloat2("Attenuation", att, .001f, .001f, 2.f))
        setAttenuation(glm::make_vec2(att));

      ImGui::Spacing();

      ImGui::Spacing();
      glm::vec3 c1 = getAmbient();
      float amb[3] = {c1.x, c1.y, c1.z};
      if (ImGui::ColorEdit3("Ambient Color", amb))
        setAmbient(glm::make_vec3(amb));

      ImGui::Spacing();
      glm::vec3 c2 = getDiffuse();
      float diff[3] = {c2.x, c2.y, c2.z};
      if (ImGui::ColorEdit3("Diffuse Color", diff))
        setDiffuse(glm::make_vec3(diff));

      ImGui::Spacing();
      glm::vec3 c3 = getSpecular();
      float spec[3] = {c3.x, c3.y, c3.z};
      if (ImGui::ColorEdit3("Specular Color", spec))
        setSpecular(glm::make_vec3(spec));

      ImGui::Spacing();
    }
  }

  void LightNode::setShape(const std::string &mesh)
  {
    mesh_ = ResourcePtr<Mesh>("meshes/" + mesh, "meshes/sphere.mesh");
    mesh_name_ = mesh;
  }

  void LightNode::setShader(const std::string &shader)
  {
    shader_ = ResourcePtr<Shader>("shaders/" + shader);
    shader_name_ = shader;
    shader_.setInitialConfig([](auto shader){
      shader->use();
      shader->set("gPosition", 3);
      shader->set("gNormal", 0);
      shader->set("gColorSpec", 1);
      shader->set("ssao", 2);
    });
  }

  void LightNode::setAmbient(const glm::vec3 &ambient)
  {
    ambient_ = ambient;
    calcRadius();
  }

  void LightNode::setDiffuse(const glm::vec3 &diffuse)
  {
    diffuse_ = diffuse;
    calcRadius();
  }

  void LightNode::setSpecular(const glm::vec3 &specular)
  {
    specular_ = specular;
    calcRadius();
  }

  void LightNode::setDirection(const glm::vec3 &direction)
  {
    direction_ = direction;
  }

  void LightNode::setIntensity(float intensity)
  {
    intensity_ = intensity;
    calcRadius();
  }

  void LightNode::setAttenuation(const glm::vec2 &attenuation)
  {
    l_att_ = attenuation.x;
    q_att_ = attenuation.y;
    calcRadius();
  }

  const std::string& LightNode::getShape()
  {
    return mesh_name_;
  }

  const std::string& LightNode::getShader()
  {
    return shader_name_;
  }

  const glm::vec3& LightNode::getAmbient()
  {
    return ambient_;
  }

  const glm::vec3& LightNode::getDiffuse()
  {
    return diffuse_;
  }

  const glm::vec3& LightNode::getSpecular()
  {
    return specular_;
  }

  const glm::vec3& LightNode::getDirection()
  {
    return direction_;
  }

  float LightNode::getIntensity()
  {
    return intensity_;
  }

  glm::vec2 LightNode::getAttenuation()
  {
    return glm::vec2(l_att_, q_att_);
  }

  void LightNode::calcRadius()
  {
    glm::vec3 light = glm::max(glm::max(ambient_, specular_), diffuse_);
    float lmax = 255.f * intensity_ * std::fmaxf(std::fmaxf(light.r, light.g), light.b);

    // Calculate the mesh scale from the attenuation and the maximum light
    float scale =
      (-l_att_ + std::sqrt(l_att_ * l_att_ - 4 * q_att_ * (1.f - lmax * (256.f / 5.f))))
      / (2 * q_att_);

    setGlobalScale(2 * scale);
  }
}
