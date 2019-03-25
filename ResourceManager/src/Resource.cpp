#include "Resource.hpp"
#include "Graphics_Engine.hpp"
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>

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
    void Shader::Set(const std::string &name, glm::vec2 *value)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(*value));
    }
    void Shader::Set(const std::string &name, float x, float y)
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y)));
    }
    void Shader::Set(const std::string &name, glm::vec3 *value)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(*value));
    }
    void Shader::Set(const std::string &name, float x, float y, float z)
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
    }
    void Shader::Set(const std::string &name, glm::vec4 *value)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(*value));
    }
    void Shader::Set(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(glm::vec4(x, y, z, w)));
    }
    void Shader::Set(const std::string &name, glm::mat4 *value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(*value));
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
        ResourcePtr r = ResourcePtr(new Shader(path, program));
        file.close();
        return r;
    }

    Texture::~Texture()
    {
        Graphics_Engine::DeleteTexture(m_Id);
    }

    ResourcePtr TextureResourceLoader::Load(std::string path)
    {
        int nrComponents, width, height;

        // Load the raw image
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data == nullptr)
        {
            LOG_ERR("TextureLoader: The image %s failed to load due to: %s", path.c_str(), stbi_failure_reason());
            data = stbi_load("../resources/textures/missing.png", &width, &height, &nrComponents, 0);
            if (data == nullptr)
                LOG_ERR("TextureLoader: The image textures/missing.png failed to load due to: %s", stbi_failure_reason());
        }

        unsigned int id = Graphics_Engine::LoadTexture(data, width, height, nrComponents);

        // Delete the image, as it is already on the GPU
        stbi_image_free(data);

        ResourcePtr r = ResourcePtr(new Texture(path, id, width, height));
        return r;
    }

    ResourcePtr MeshResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        // Read the file into a vector
        std::string line;
        std::vector<float> v;
        int vertices = 0;
        while (std::getline(file, line))
        {
            float value;
            bool valid_line = false;
            std::stringstream ss(line);
            while (ss >> value)
            {
                v.push_back(value);
                valid_line = true;
            }
            if (valid_line) vertices++;
        }

        // Create the Mesh
        unsigned int id = Graphics_Engine::LoadMesh(v);

        ResourcePtr r = ResourcePtr(new MeshResource(path, id, vertices));
        return r;
    }

    MeshResource::~MeshResource()
    {
        Graphics_Engine::DeleteMesh(m_Id);
    }
}
