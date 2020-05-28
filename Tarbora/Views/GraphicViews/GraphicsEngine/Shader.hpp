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

#ifndef __SHADER_H_
#define __SHADER_H_

#include "../../../Framework/Global.hpp"
#include "../../../Framework/ResourceManager/Resource.hpp"

namespace Tarbora {
  class Shader : public Resource
  {
    friend class ShaderResourceLoader;
  public:
    ~Shader();

    void use() const;
    unsigned int getId() const { return id_; }

    void set(const std::string &name, bool value);
    void set(const std::string &name, int value);
    void set(const std::string &name, float value);
    void set(const std::string &name, const glm::vec2 &value);
    void set(const std::string &name, float x, float y);
    void set(const std::string &name, const glm::vec3 &value);
    void set(const std::string &name, float x, float y, float z);
    void set(const std::string &name, const glm::vec4 &value);
    void set(const std::string &name, float x, float y, float z, float w);
    void set(const std::string &name, const glm::mat4 &value);

  private:
    Shader(const std::string &name) : Resource(name) {}
    void deleteShader();

    unsigned int compileShader(const std::string &path, const std::string &type);
    unsigned int compileShaderFile(const std::string &type, const std::string &code);
    void linkProgram(unsigned int *ids);

    unsigned int id_;
  };

  //! \cond HIDDEN_SYMBOLS
  class ShaderResourceLoader : public ResourceLoader
  {
    friend class ResourceManager;
  private:
    virtual const std::string getPattern() override { return ".shader.lua"; };
    virtual std::shared_ptr<Resource> load(const std::string &path) override;
  };
  //! \endcond
}

#endif // __SHADER_H_
