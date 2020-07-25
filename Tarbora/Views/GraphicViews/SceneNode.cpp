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

namespace Tarbora {
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
    guiTransformFixedSize();
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

      if (ImGui::RadioButton("Translate", current_guizmo_op_ == ImGuizmo::TRANSLATE))
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

      ImGuizmo::Manipulate(
        glm::value_ptr(scene_->getView()), glm::value_ptr(scene_->getProjection()),
        current_guizmo_op_, current_guizmo_mode_ == 2 ? ImGuizmo::WORLD : ImGuizmo::LOCAL,
        transform
      );

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

        LOG_DEBUG("%s", glm::to_string(gpos).c_str());

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

  SceneNodePtr SceneNode::addChild(SceneNodePtr child)
  {
    children_[child->name] = child;
    child->parent_ = this;
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

  void MaterialNode::draw(Scene *scene)
  {
    scene->getRenderQueue()->pushMaterial(material_);
  }

  void MaterialNode::afterDraw(Scene *scene)
  {
    scene->getRenderQueue()->popMaterial();
  }

  void MaterialNode::load(const LuaTable &table, NodeMap *map)
  {
    SceneNode::load(table, map);
    setMaterial(table.get<std::string>("material"));
  }

  void MaterialNode::setMaterial(const std::string &name)
  {
    material_ = ResourcePtr<Material>(
      "materials/" + name,
      "materials/missing.mat.lua");
    material_name_ = name;
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
        color_primary_,
        color_secondary_,
        color_detail_,
        color_detail2_
      );
    }
  }

  void MeshNode::drawGuiEditor()
  {
    guiTransform();
    ImGui::Spacing();
    guiProperties();
    ImGui::Spacing();
    guiColors();
    ImGui::Spacing();
    guiTexture();
  }

  void MeshNode::guiProperties()
  {
    if (ImGui::CollapsingHeader("Properties"))
    {
      ImGui::Spacing();
      bool animated = (getType() == "animated" && new_type_ == "") ||
        new_type_ == "animated";
      if (ImGui::Checkbox("Animated", &animated))
      {
        new_type_ = animated ? "animated" : "mesh";
      }

      ImGui::Spacing();
    }
  }

  void MeshNode::guiColors()
  {
    if (ImGui::CollapsingHeader("Colors"))
    {
      ImGui::Spacing();
      glm::vec3 c1 = getColorPrimary();
      c1 /= 255.f;
      float primary[3] = {c1.x, c1.y, c1.z};
      if (ImGui::ColorEdit3("Color Primary", &primary[0]))
        setColorPrimary(glm::make_vec3(primary)*255.f);

      ImGui::Spacing();
      glm::vec3 c2 = getColorSecondary();
      c2 /= 255.f;
      float secondary[3] = {c2.x, c2.y, c2.z};
      if (ImGui::ColorEdit3("Color Secondary", &secondary[0]))
        setColorSecondary(glm::make_vec3(secondary)*255.f);

      ImGui::Spacing();
      glm::vec3 c3 = getColorDetail();
      c3 /= 255.f;
      float detail[3] = {c3.x, c3.y, c3.z};
      if (ImGui::ColorEdit3("Color Detail", &detail[0]))
        setColorDetail(glm::make_vec3(detail)*255.f);

      ImGui::Spacing();
      glm::vec3 c4 = getColorDetail2();
      c4 /= 255.f;
      float detail2[3] = {c4.x, c4.y, c4.z};
      if (ImGui::ColorEdit3("Color Detail 2", &detail2[0]))
        setColorDetail2(glm::make_vec3(detail2)*255.f);

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

    setUvMap(table.get<glm::vec3>("uv_map", true));
    setColorPrimary(table.get<glm::vec3>("color_primary", glm::vec3(255.f), true));
    setColorSecondary(table.get<glm::vec3>("color_secondary", glm::vec3(255.f), true));
    setColorDetail(table.get<glm::vec3>("color_detail1", glm::vec3(255.f), true));
    setColorDetail2(table.get<glm::vec3>("color_detail2", glm::vec3(255.f), true));
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

  void MeshNode::setColorPrimary(const glm::tvec3<unsigned char> &color)
  {
    color_primary_ = color;
  }

  void MeshNode::setColorSecondary(const glm::tvec3<unsigned char> &color)
  {
    color_secondary_ = color;
  }

  void MeshNode::setColorDetail(const glm::tvec3<unsigned char> &color)
  {
    color_detail_ = color;
  }

  void MeshNode::setColorDetail2(const glm::tvec3<unsigned char> &color)
  {
    color_detail2_ = color;
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

  const glm::tvec3<unsigned char>& MeshNode::getColorPrimary()
  {
    return color_primary_;
  }

  const glm::tvec3<unsigned char>& MeshNode::getColorSecondary()
  {
    return color_secondary_;
  }

  const glm::tvec3<unsigned char>& MeshNode::getColorDetail()
  {
    return color_detail_;
  }

  const glm::tvec3<unsigned char>& MeshNode::getColorDetail2()
  {
    return color_detail2_;
  }

  void AnimatedNode::resetAll()
  {
    position_anim_ = glm::vec3(0.f);
    orientation_anim_ = glm::quat({0.f, 0.f, 0.f});
    scale_anim_ = glm::vec3(0.f);
    global_scale_anim_ = 1.f;
    uv_map_anim_ = glm::tvec2<unsigned short>(0);
    color_primary_anim_ = glm::tvec3<unsigned char>(0);
    color_secondary_anim_ = glm::tvec3<unsigned char>(0);
    color_detail_anim_ = glm::tvec3<unsigned char>(0);
    color_detail2_anim_ = glm::tvec3<unsigned char>(0);
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
        color_primary_ + color_primary_anim_,
        color_secondary_ + color_secondary_anim_,
        color_detail_ + color_detail_anim_,
        color_detail2_ + color_detail2_anim_
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

  void AnimatedNode::setColorPrimaryAnimation(const glm::tvec3<unsigned char> &color)
  {
    color_primary_anim_ = color;
  }

  void AnimatedNode::setColorSecondaryAnimation(const glm::tvec3<unsigned char> &color)
  {
    color_secondary_anim_ = color;
  }

  void AnimatedNode::setColorDetailAnimation(const glm::tvec3<unsigned char> &color)
  {
    color_detail_anim_ = color;
  }

  void AnimatedNode::setColorDetail2Animation(const glm::tvec3<unsigned char> &color)
  {
    color_detail2_anim_ = color;
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

  const glm::tvec3<unsigned char>& AnimatedNode::getColorPrimaryAnimation()
  {
    return color_primary_anim_;
  }

  const glm::tvec3<unsigned char>& AnimatedNode::getColorSecondaryAnimation()
  {
    return color_secondary_anim_;
  }

  const glm::tvec3<unsigned char>& AnimatedNode::getColorDetailAnimation()
  {
    return color_detail_anim_;
  }

  const glm::tvec3<unsigned char>& AnimatedNode::getColorDetail2Animation()
  {
    return color_detail2_anim_;
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

    setAttenuation(table.get<glm::vec3>("attenuation", glm::vec3(1.f), true));
  }

  void LightNode::drawGuiEditor()
  {
    guiTransformFixedSize();
    ImGui::Spacing();
    guiLight();
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

  glm::vec2 LightNode::getAttenuation()
  {
    return glm::vec2(l_att_, q_att_);
  }

  void LightNode::calcRadius()
  {
    glm::vec3 light = glm::max(glm::max(ambient_, specular_), diffuse_);
    float lmax = 255.f * std::fmaxf(std::fmaxf(light.r, light.g), light.b);

    // Calculate the mesh scale from the attenuation and the maximum light
    float scale =
      (-l_att_ + std::sqrt(l_att_ * l_att_ - 4 * q_att_ * (1.f - lmax * (256.f / 5.f))))
      / (2 * q_att_);

    setGlobalScale(2 * scale);
  }
}
