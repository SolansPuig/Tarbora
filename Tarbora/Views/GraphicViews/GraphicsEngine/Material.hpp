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

#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "Texture.hpp"
#include "Shader.hpp"

namespace Tarbora {
  typedef std::string MaterialId;

  class Material : public Resource
  {
    friend class MaterialResourceLoader;
  public:
    void bind(const glm::mat4 &projection, const glm::mat4 &view) const;

    ResourcePtr<Texture> getAlbedo() const { return albedo_; }
    ResourcePtr<Texture> getSpecular() const { return specular_; }
    ResourcePtr<Texture> getColorTint() const { return color_tint_; }
    ResourcePtr<Shader> getShader() const { return shader_; }
    const MaterialId& getId() const { return name_; }

    void setScreenSize(glm::vec2 size) { size_ = size; }

  private:
    Material(const std::string &name)
      : Resource(name) {}

    ResourcePtr<Texture> albedo_;
    ResourcePtr<Texture> specular_;
    ResourcePtr<Texture> color_tint_;
    ResourcePtr<Shader> shader_;
    int pixel_density_;
    glm::vec2 size_;
  };

  //! \cond HIDDEN_SYMBOLS
  class MaterialResourceLoader : public ResourceLoader
  {
    friend class ResourceManager;
  private:
    virtual const std::string getPattern() override { return ".mat.lua"; };
    virtual std::shared_ptr<Resource> load(const std::string &path) override;
  };
  //! \endcond
}

#endif // __MATERIAL_H_
