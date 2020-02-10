#include "GraphicsEngine.hpp"
#include "../../../Messages/BasicMessages.hpp"
#include "../../../Framework/ResourceManager/inc/Lua.hpp"

namespace Tarbora {
    Shader::~Shader()
    {
        Delete();
    }

    void Shader::Use() const
    {
        glUseProgram(m_Id);
    }

    void Shader::Set(const std::string name, int value)
    {
        glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
    }

    void Shader::Set(const std::string name, float value)
    {
        glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
    }

    void Shader::Set(const std::string name, const glm::vec2 &value)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y)));
    }

    void Shader::Set(const std::string name, const glm::vec3 &value)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y, float z)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
    }

    void Shader::Set(const std::string name, const glm::vec4 &value)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y, float z, float w)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w)));
    }

    void Shader::Set(const std::string name, const glm::mat4 &value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    unsigned int Shader::CompileShader(const std::string &path, const std::string &type)
    {
        if (path != "")
        {
            std::ifstream file(ResourceManager::GetResourceFolder() + path.c_str());
            if (file)
            {
                std::string code = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                file.close();
                return CompileShaderFile(type, code);
            }
            LOG_ERR("ShaderCompiler: Failed to load file %s for shader type %s.", path.c_str(), type.c_str());
        }
        else
        {
            if (type == "vertex" || type == "fragment")
                LOG_ERR("ShaderCompiler: Shader type %s is required.", type.c_str());
        }

        return 0;
    }

    unsigned int Shader::CompileShaderFile(const std::string &type, const std::string &code)
    {
        // Create and compile the shader
        unsigned int id = 0;
        if (type == "vertex") id = glCreateShader(GL_VERTEX_SHADER);
        else if (type == "tes_control") id = glCreateShader(GL_TESS_CONTROL_SHADER);
        else if (type == "tes_eval") id = glCreateShader(GL_TESS_EVALUATION_SHADER);
        else if (type == "geometry") id = glCreateShader(GL_GEOMETRY_SHADER);
        else if (type == "fragment") id = glCreateShader(GL_FRAGMENT_SHADER);
        else if (type == "compute") id = glCreateShader(GL_COMPUTE_SHADER);
        else LOG_ERR("ShaderCompiler: Shader type %s not recognized", type.c_str());
        const char *c = code.c_str();
        glShaderSource(id, 1, &c, NULL);
        glCompileShader(id);
        // Check for errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            LOG_ERR("ShaderCompiler: Error while compiling %s shader. \n %s", type.c_str(), infoLog);
        }

        return id;
    }

    void Shader::LinkProgram(unsigned int *ids)
    {
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
            LOG_ERR("ShaderCompiler: Error while linking program. \n %s", infoLog);
        }

        // Delete all the shaders, as they are linked to the program and no longer needed
        for (int i = 0; i < 6; i++)
            glDeleteShader(ids[i]);

        m_Id = id;
    }

    void Shader::Delete()
    {
        glDeleteProgram(m_Id);
    }

    std::shared_ptr<Resource> ShaderResourceLoader::Load(std::string path)
    {
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
                vertex = resource.Get<std::string>("vertex");
                tesControl = resource.Get<std::string>("tes_control", tesControl, true);
                tesEval = resource.Get<std::string>("tes_eval", tesEval, true);
                geometry = resource.Get<std::string>("geometry", geometry, true);
                fragment = resource.Get<std::string>("fragment");
                compute = resource.Get<std::string>("compute", compute, true);
            }
        }

        // Create the shader resource
        std::shared_ptr<Shader> shader(new Shader(path));

        // Read each entry and create and compile the shaders
        unsigned int ids[6];
        ids[0] = shader->CompileShader(vertex, "vertex");
        ids[1] = shader->CompileShader(tesControl, "tes_control");
        ids[2] = shader->CompileShader(tesEval, "tes_eval");
        ids[3] = shader->CompileShader(geometry, "geometry");
        ids[4] = shader->CompileShader(fragment, "fragment");
        ids[5] = shader->CompileShader(compute, "compute");

        // Link the program
        shader->LinkProgram(ids);

        return shader;
    }
}
