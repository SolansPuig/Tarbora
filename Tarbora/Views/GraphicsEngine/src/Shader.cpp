#include "../inc/GraphicsEngine.hpp"
#include "../../GraphicViews/inc/GraphicView.hpp"
#include "../../../Messages/BasicMessages.hpp"

#define GAMEVIEW(MODULE) static_cast<GraphicView*>(MODULE)

namespace Tarbora {
    Shader::Shader(Module *m, std::string name) : Resource(m, name)
    {
        m_Module->GetMessageManager()->Subscribe("update_view", [this](MessageSubject subject, MessageBody * body)
        {
            MatrixBody m = body->GetContent<MatrixBody>();
            Use();
            Set("view", Mat4toGLM(m.matrix()));
        }, true);

        m_Module->GetMessageManager()->Subscribe("update_projection", [this](MessageSubject subject, MessageBody * body)
        {
            MatrixBody m = body->GetContent<MatrixBody>();
            Use();
            Set("projection", Mat4toGLM(m.matrix()));
        }, true);
    }

    Shader::~Shader()
    {
        Delete();
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

    unsigned int Shader::CompileShader(raw_json j, const char* type)
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
            id = CompileShaderFile(type, code);
        } else {
            if (type == std::string("vertex") || type == std::string("fragment"))
                LOG_ERR("ShaderCompiler: Shader type %s is required.", type);
            return 0;
        }
        return id;
    }

    unsigned int Shader::CompileShaderFile(std::string type, const char *code)
    {
        // Create and compile the shader
        unsigned int id = 0;
        if (type == std::string("vertex")) id = glCreateShader(GL_VERTEX_SHADER);
        else if (type == std::string("tes_control")) id = glCreateShader(GL_TESS_CONTROL_SHADER);
        else if (type == std::string("tes_eval")) id = glCreateShader(GL_TESS_EVALUATION_SHADER);
        else if (type == std::string("geometry")) id = glCreateShader(GL_GEOMETRY_SHADER);
        else if (type == std::string("fragment")) id = glCreateShader(GL_FRAGMENT_SHADER);
        else if (type == std::string("compute")) id = glCreateShader(GL_COMPUTE_SHADER);
        else LOG_ERR("ShaderCompiler: Shader type %s not recognized", type.c_str());
        glShaderSource(id, 1, &code, NULL);
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

        // Create the shader resource
        std::shared_ptr<Shader> shader(new Shader(m_Module, path));

        // Read each entry and create and compile the shaders
        unsigned int ids[6];
        ids[0] = shader->CompileShader(j, "vertex");
        ids[1] = shader->CompileShader(j, "tes_control");
        ids[2] = shader->CompileShader(j, "tes_eval");
        ids[3] = shader->CompileShader(j, "geometry");
        ids[4] = shader->CompileShader(j, "fragment");
        ids[5] = shader->CompileShader(j, "compute");

        // Link the program
        shader->LinkProgram(ids);

        file.close();
        return shader;
    }
}
