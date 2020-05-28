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

#include "GraphicsEngine.hpp"
#include "../../../Messages/BasicMessages.hpp"
#include "../../../Framework/ResourceManager/Lua.hpp"

namespace Tarbora {
  Shader::~Shader()
  {
    deleteShader();
  }

  void Shader::use() const
  {
    glUseProgram(id_);
  }

  void Shader::set(const std::string &name, int value)
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }

  void Shader::set(const std::string &name, float value)
  {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }

  void Shader::set(const std::string &name, const glm::vec2 &value)
  {
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
  }

  void Shader::set(const std::string &name, float x, float y)
  {
    glUniform2fv(
      glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y))
    );
  }

  void Shader::set(const std::string &name, const glm::vec3 &value)
  {
    glUniform3fv(
      glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value)
    );
  }

  void Shader::set(const std::string &name, float x, float y, float z)
  {
    glUniform3fv(
      glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z))
    );
  }

  void Shader::set(const std::string &name, const glm::vec4 &value)
  {
    glUniform4fv(
      glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value)
    );
  }

  void Shader::set(const std::string &name, float x, float y, float z, float w)
  {
    glUniform4fv(
      glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w))
    );
  }

  void Shader::set(const std::string &name, const glm::mat4 &value)
  {
    glUniformMatrix4fv(
      glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)
    );
  }

  unsigned int Shader::compileShader(const std::string &path, const std::string &type)
  {
    ZoneScoped;

    if (path != "")
    {
      std::ifstream file(ResourceManager::getResourceFolder() + path.c_str());
      if (file)
      {
        const std::string code = std::string(
          std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()
        );
        file.close();
        return compileShaderFile(type, code);
      }
      LOG_ERR(
        "Shader %s: Failed to load file %s for shader type %s.",
        name_.c_str(), path.c_str(), type.c_str()
      );
    }
    else
    {
      if (type == "vertex" || type == "fragment")
        LOG_ERR("Shader %s: Shader type %s is required.", name_.c_str(), type.c_str());
    }

    return 0;
  }

  unsigned int Shader::compileShaderFile(const std::string &type, const std::string &code)
  {
    ZoneScoped;

    // Create and compile the shader
    unsigned int id = 0;
    if (type == "vertex") id = glCreateShader(GL_VERTEX_SHADER);
    else if (type == "tes_control") id = glCreateShader(GL_TESS_CONTROL_SHADER);
    else if (type == "tes_eval") id = glCreateShader(GL_TESS_EVALUATION_SHADER);
    else if (type == "geometry") id = glCreateShader(GL_GEOMETRY_SHADER);
    else if (type == "fragment") id = glCreateShader(GL_FRAGMENT_SHADER);
    else if (type == "compute") id = glCreateShader(GL_COMPUTE_SHADER);
    else
    {
      LOG_ERR("Shader %s: Shader type %s not recognized", name_.c_str(), type.c_str());
    }

    const char *c = code.c_str();
    glShaderSource(id, 1, &c, NULL);
    glCompileShader(id);
    // Check for errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(id, 512, NULL, infoLog);
      LOG_ERR(
        "Shader %s: Error while compiling %s shader. \n %s",
        name_.c_str(), type.c_str(), infoLog
      );
    }

    return id;
  }

  void Shader::linkProgram(unsigned int *ids)
  {
    ZoneScoped;

    // Attach all the shaders if they are valid and link the program
    unsigned int id = glCreateProgram();
    for (int i = 0; i < 6; i++)
      if (ids[i] != 0)
        glAttachShader(id, ids[i]);
    glLinkProgram(id);

    // Check for errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id, 512, NULL, infoLog);
      LOG_ERR("Shader %s: Error while linking program. \n %s", name_.c_str(), infoLog);
    }

    // Delete all the shaders, as they are linked to the program and no longer needed
    for (int i = 0; i < 6; i++)
      glDeleteShader(ids[i]);

    id_ = id;
  }

  void Shader::deleteShader()
  {
    glDeleteProgram(id_);
  }

  std::shared_ptr<Resource> ShaderResourceLoader::load(const std::string &path)
  {
    ZoneScoped;

    std::string vertex = "empty.vert";
    std::string tesControl = "";
    std::string tesEval = "";
    std::string geometry = "";
    std::string fragment = "empty.frag";
    std::string compute = "";

    {
      std::ifstream file(path);
      if (file) {
        LuaScript resource(path);
        vertex = resource.get<std::string>("vertex");
        tesControl = resource.get<std::string>("tes_control", tesControl, true);
        tesEval = resource.get<std::string>("tes_eval", tesEval, true);
        geometry = resource.get<std::string>("geometry", geometry, true);
        fragment = resource.get<std::string>("fragment");
        compute = resource.get<std::string>("compute", compute, true);
      }
    }

    // Create the shader resource
    std::shared_ptr<Shader> shader(new Shader(path));

    // Read each entry and create and compile the shaders
    unsigned int ids[6];
    ids[0] = shader->compileShader(vertex, "vertex");
    ids[1] = shader->compileShader(tesControl, "tes_control");
    ids[2] = shader->compileShader(tesEval, "tes_eval");
    ids[3] = shader->compileShader(geometry, "geometry");
    ids[4] = shader->compileShader(fragment, "fragment");
    ids[5] = shader->compileShader(compute, "compute");

    // Link the program
    shader->linkProgram(ids);

    return shader;
  }
}
