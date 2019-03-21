#include "Resource.hpp"
#include "Graphics_Engine.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Tarbora {
    ResourcePtr TextResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        std::string s = std::string (std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        ResourcePtr r = ResourcePtr(new TextResource(path, s));
        file.close();
        return r;
    }

    ResourcePtr JsonResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();
        ResourcePtr r = ResourcePtr(new JsonResource(path, json::parse(file)));
        file.close();
        return r;
    }

    Shader::~Shader()
    {
        Graphics_Engine::DeleteProgram(m_Id);
    }

    void Shader::Use()
    {
        glUseProgram(m_Id);
    }

    void Shader::Set(const std::string &name, int value)
    {
        glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
    }
    void Shader::Set(const std::string &name, float value)
    {
        glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
    }
    void Shader::Set(const std::string &name, glm::vec2 value)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }
    void Shader::Set(const std::string &name, float x, float y)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y)));
    }
    void Shader::Set(const std::string &name, glm::vec3 value)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }
    void Shader::Set(const std::string &name, float x, float y, float z)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
    }
    void Shader::Set(const std::string &name, glm::vec4 value)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }
    void Shader::Set(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w)));
    }
    void Shader::Set(const std::string &name, glm::mat4 value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    unsigned int CompileShader(json j, const char* type)
    {
        unsigned int id;

        if (j.find(type) != j.end() && j[type] != "")
        {
            // Read the shader file
            std::ifstream file;
            std::string path = j[type];
            file.open(ResourceManager::GetResourceFolder() + path.c_str());
            if (file.fail()) {
                LOG_ERR("ShaderCompiler: Failed to load file %s for shader type %s.", path.c_str(), type);
                return 0;
            }
            std::string s = std::string (std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            file.close();
            const char *code = s.c_str();
            // Compile it
            id = Graphics_Engine::CompileShader(type, code);
        } else {
            if (type == std::string("vertex") || type == std::string("fragment"))
                LOG_ERR("ShaderCompiler: Shader type %s is required.", type);
            return 0;
        }
        return id;
    }

    ResourcePtr ShaderResourceLoader::Load(std::string path)
    {
        // Open the Json containing all the different shaders for that program
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();
        json j = json::parse(file);

        // Read each entry and create and compile the shaders
        unsigned int ids[6];
        ids[0] = CompileShader(j, "vertex");
        ids[1] = CompileShader(j, "tes_control");
        ids[2] = CompileShader(j, "tes_eval");
        ids[3] = CompileShader(j, "geometry");
        ids[4] = CompileShader(j, "fragment");
        ids[5] = CompileShader(j, "compute");

        // Link the program
        unsigned int program = Graphics_Engine::LinkProgram(ids);

        ResourcePtr r = ResourcePtr(new JsonResource(path, program));
        file.close();
        return r;
    }
}
