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

#ifndef __SCENENODE_H_
#define __SCENENODE_H_

#include "GraphicView.hpp"

namespace Tarbora {
  class Scene;
  class SceneNode;
  typedef std::shared_ptr<SceneNode> SceneNodePtr;
  typedef std::unordered_map<std::string, SceneNodePtr> NodeMap;
  typedef glm::tvec3<unsigned char> Color;
  typedef std::pair<std::string, Color> NamedColor;

  class ActorModel;
 
  struct TransformProps {
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale{0.f, 0.f, 0.f};
    float global_scale{0.f};
  };

  class SceneNode
  {
  public:
    SceneNode(const ActorId &id, const std::string &name) : owner(id), name(name) {}
    SceneNode(const SceneNode &other);
    ~SceneNode();

    virtual const std::string getType() const = 0;

    virtual void update(Scene *scene, float delta_time);
    virtual void drawChildren(Scene *scene);
    virtual void draw(Scene *) {};
    virtual void afterDraw(Scene *) {}

    virtual SceneNodePtr clone() const = 0;

    virtual void drawGuiEditor();
    void guiName();
    void guiTransform(bool fixedSize=false);
    inline void guiTransformFixedSize() { guiTransform(true); }

    virtual void load(const LuaTable &table, NodeMap *map=nullptr);
    virtual void write(LuaFile *file);
    virtual void writeName(LuaFile *file);
    virtual void writeTransform(LuaFile *file, bool fixedSize=false);
    virtual void writeTransformFixedSize(LuaFile *file) { writeTransform(file, true); }
    virtual void writeChildren(LuaFile *file);

    SceneNodePtr addChild(SceneNodePtr child);
    virtual SceneNodePtr getChild(const std::string &name);
    SceneNode* getParent() { return parent_; }
    bool removeChild(const std::string &name);

    bool size() { return children_.size(); }
    auto begin() { return children_.begin(); }
    auto end() { return children_.end(); }

    void setRenderPass(RenderPass pass);
    void setPosition(const glm::vec3 &position);
    void setOrientation(const glm::vec3 &orientation);
    void setOrientation(const glm::quat &orientation);
    void setScale(const glm::vec3 &scale);
    void setGlobalScale(float scale);
    void setOrigin(const glm::vec3 &origin);

    RenderPass getRenderPass();
    const glm::vec3& getPosition();
    glm::vec3 getEulerOrientation();
    const glm::quat& getOrientation();
    const glm::vec3& getScale();
    float getGlobalScale();
    glm::vec3 getOrigin();
    glm::vec3 getRawOrigin();


    void setLocal(const TransformProps &props);
    TransformProps getLocalDecomposed();
    const glm::mat4& getLocal();
    const glm::mat4& getWorld();


    ActorId owner;
    std::string name;

    //temp
    void setNewNodeType(const std::string &type) { new_type_ = type; }
    const std::string& getNewNodeType() { return new_type_; }


  protected:
    Scene *scene_{nullptr};
    SceneNode *parent_{nullptr};
    NodeMap children_;
    ActorModel *model_{nullptr};

    RenderPass render_pass_{RenderPass::Static};

    glm::vec3 origin_{0.f, 0.f, 0.f};
    glm::vec3 position_{0.f, 0.f, 0.f};
    glm::quat orientation_{glm::vec3(0.f, 0.f, 0.f)};
    glm::vec3 scale_{1.f, 1.f, 1.f};
    float global_scale_{1.f};

    glm::mat4 local_{1.f};
    glm::mat4 world_{1.f};
    glm::mat4 transform_{1.f};

    void setDirty();
    void setVeryDirty();
    virtual void fixDirty();
    virtual void fixLocal();
    virtual void fixWorld();
    virtual void fixTransform();
    bool dirty_{true};
    bool very_dirty_{true};

    //temp
    std::string new_type_;
  };

  class RootNode : public SceneNode
  {
  public:
    RootNode() : SceneNode("", "Root") {}
    virtual ~RootNode() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<RootNode>(*this);
    }

    virtual const std::string getType() const { return "root"; }
  };

  class Camera : public SceneNode
  {
  public:
    Camera(const ActorId &id, const std::string &name) : SceneNode(id, name) {}
    virtual ~Camera() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<Camera>(*this);
    }
   
    virtual const std::string getType() const { return "camera"; }

    const glm::mat4& getView();

  private:
    virtual void fixDirty();
    virtual void fixView();
    glm::mat4 view_;
  };

  class MaterialNode : public SceneNode
  {
  public:
    MaterialNode(const ActorId &id, const std::string &name) : SceneNode(id, name) {}
    MaterialNode(const MaterialNode &other);
    virtual ~MaterialNode() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<MaterialNode>(*this);
    }

    virtual const std::string getType() const { return "material"; }

    virtual void draw(Scene *scene);
    virtual void afterDraw(Scene *scene);

    virtual void drawGuiEditor();
    void guiProperties();
    virtual void load(const LuaTable &table, NodeMap *map=nullptr);
    virtual void write(LuaFile *file);
    void writeProperties(LuaFile *file);

    void setMaterial(const std::string &name);
    const std::string& getMaterial() { return material_name_; }

  protected:
    ResourcePtr<Material> material_{"materials/white.mat.lua"};
    std::string material_name_{"white.mat.lua"};
  };

  class MeshNode : public SceneNode
  {
  public:
    MeshNode(const ActorId &id, const std::string &name) : SceneNode(id, name) {}
    MeshNode(const MeshNode &other);
    virtual ~MeshNode() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<MeshNode>(*this);
    }

    virtual const std::string getType() const { return "mesh"; }

    virtual void draw(Scene *scene);

    virtual void drawGuiEditor();
    void guiProperties();
    void guiColors();
    void guiTexture();
    virtual void load(const LuaTable &table, NodeMap *map=nullptr);
    virtual void write(LuaFile *file);
    void writeMesh(LuaFile *file);

    void setShape(const std::string &mesh);

    void setUvMap(const glm::tvec2<unsigned short> &uv);
    void setMeshSize(const glm::vec3 &size);
    void setTextureSize(const glm::vec3 &size);

    void setColor(unsigned int i, const std::string &color);

    const std::string& getShape();

    const glm::tvec2<unsigned short>& getUvMap();
    const glm::vec3& getMeshSize();
    const glm::vec3& getTextureSize();
    bool getAutoTextureSize();

    const std::shared_ptr<NamedColor> getColor(unsigned int i);

  protected:
    ResourcePtr<Mesh> mesh_{"meshes/cube.mesh"};
    std::string mesh_name_{"cube.mesh"};

    glm::tvec2<unsigned short> uv_map_{0};
    glm::vec3 mesh_size_{0.f};
    glm::vec3 texture_size_{0.f};
    bool auto_texture_size_{true};

    std::weak_ptr<NamedColor> color_[4];
  };

  class AnimatedNode : public MeshNode
  {
  public:
    AnimatedNode(const ActorId &id, const std::string &name) : MeshNode(id, name) {}
    virtual ~AnimatedNode() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<AnimatedNode>(*this);
    }

    virtual const std::string getType() const { return "animated"; }

    virtual void draw(Scene *scene);

    void resetAll();

    void setPositionAnimation(const glm::vec3 &position);
    void setOrientationAnimation(const glm::vec3 &orientation);
    void setOrientationAnimation(const glm::quat &orientation);
    void setScaleAnimation(const glm::vec3 &scale);
    void setGlobalScaleAnimation(float scale);
    void setUvMapAnimation(const glm::tvec2<unsigned short> &uv);
    void setColorAnimation(unsigned int i, const Color &color);

    const glm::vec3& getPositionAnimation();
    const glm::quat& getOrientationAnimation();
    const glm::vec3& getScaleAnimation();
    float getGlobalScaleAnimation();
    const glm::tvec2<unsigned short>& getUvMapAnimation();
    const Color& getColorAnimation(unsigned int i);

  protected:
    glm::vec3 position_anim_{0.f, 0.f, 0.f};
    glm::quat orientation_anim_{glm::vec3(0.f, 0.f, 0.f)};
    glm::vec3 scale_anim_{0.f, 0.f, 0.f};
    float global_scale_anim_{1.f};
    glm::tvec2<unsigned short> uv_map_anim_{0};
    Color color_anim_[4];

    virtual void fixLocal();
    virtual void fixTransform();
  };

  class LightNode : public SceneNode
  {
  public:
    LightNode(const ActorId &id, const std::string &name) : SceneNode(id, name) {}
    LightNode(const LightNode &other);
    virtual ~LightNode() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<LightNode>(*this);
    }

    virtual const std::string getType() const { return "light"; }

    virtual void draw(Scene *scene);

    virtual void drawGuiEditor();
    void guiProperties();
    void guiLight();
    virtual void load(const LuaTable &table, NodeMap *map=nullptr);
    virtual void write(LuaFile *file);
    void writeLight(LuaFile *file);

    void setShape(const std::string &mesh);
    void setShader(const std::string &shader);

    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);

    void setDirection(const glm::vec3 &direction);
    void setIntensity(float intensity);
    void setAttenuation(const glm::vec2 &attenuation);

    const std::string& getShape();
    const std::string& getShader();

    const glm::vec3& getAmbient();
    const glm::vec3& getDiffuse();
    const glm::vec3& getSpecular();

    const glm::vec3& getDirection();
    float getIntensity();
    glm::vec2 getAttenuation();

  protected:
    ResourcePtr<Mesh> mesh_{"meshes/sphere.mesh"};
    std::string mesh_name_{"sphere.mesh"};

    ResourcePtr<Shader> shader_{"shaders/point_light.shader.lua"};
    std::string shader_name_{"point_light.shader.lua"};

    glm::vec3 ambient_{1.f, 1.f, 1.f};
    glm::vec3 diffuse_{1.f, 1.f, 1.f};
    glm::vec3 specular_{1.f, 1.f, 1.f};

    glm::vec3 direction_{1.f, 1.f, 1.f};

    float intensity_{1.f};
    float l_att_{1.f}; // Linear attenuation
    float q_att_{1.f}; // Quadratic attenuation

    void calcRadius();
  };
}

#endif // __SCENENODE_H_
