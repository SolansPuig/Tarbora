#include "../inc/GraphicsEngine.hpp"
#include "../../../Framework/Utility/inc/json.hpp"
typedef nlohmann::json raw_json;
#include "../../GraphicViews/inc/GraphicView.hpp"

#define GAMEVIEW(APP) static_cast<GraphicView*>(APP)

namespace Tarbora {
    Shader::~Shader()
    {
        GAMEVIEW(app)->GraphicsEngine()->DeleteProgram(m_Id);
    }

    void Shader::Use()
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

    void Shader::Set(const std::string name, glm::vec2 &value)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y)));
    }

    void Shader::Set(const std::string name, glm::vec3 &value)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y, float z)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
    }

    void Shader::Set(const std::string name, glm::vec4 &value)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::Set(const std::string name, float x, float y, float z, float w)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w)));
    }

    void Shader::Set(const std::string name, glm::mat4 &value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    unsigned int CompileShader(raw_json j, const char* type, std::shared_ptr<GraphicsEngineImpl> graphics_engine)
    {
        unsigned int id;

        if (j[type].is_string() && j[type] != "")
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
            id = graphics_engine->CompileShader(type, code);
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

        raw_json j;
        try
        {
            j = raw_json::parse(file);
        }
        catch (raw_json::parse_error& e)
        {
            LOG_ERR("JsonResourceLoader: Trying to parse file \"%s\" found exception: \n \"%s\"", path.c_str(), e.what());
            file.close();
            return ResourcePtr();
        }

        // Read each entry and create and compile the shaders
        std::shared_ptr<GraphicsEngineImpl> graphics_engine = GAMEVIEW(app)->GraphicsEngine();
        unsigned int ids[6];
        ids[0] = CompileShader(j, "vertex", graphics_engine);
        ids[1] = CompileShader(j, "tes_control", graphics_engine);
        ids[2] = CompileShader(j, "tes_eval", graphics_engine);
        ids[3] = CompileShader(j, "geometry", graphics_engine);
        ids[4] = CompileShader(j, "fragment", graphics_engine);
        ids[5] = CompileShader(j, "compute", graphics_engine);

        // Link the program
        unsigned int program = GAMEVIEW(app)->GraphicsEngine()->LinkProgram(ids);
        ResourcePtr r = ResourcePtr(new Shader(app, path, program));
        file.close();
        return r;
    }
}
