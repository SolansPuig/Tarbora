#pragma once
#include "../../ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Shader : public Resource
    {
    public:
        Shader(std::string name, unsigned int id) : Resource(name), m_Id(id) {}
        ~Shader();
        void Use();
        unsigned int GetId() const { return m_Id; }
        void Set(const std::string name, bool value);
        void Set(const std::string name, int value);
        void Set(const std::string name, float value);
        void Set(const std::string name, glm::vec2 &value);
        void Set(const std::string name, float x, float y);
        void Set(const std::string name, glm::vec3 &value);
        void Set(const std::string name, float x, float y, float z);
        void Set(const std::string name, glm::vec4 &value);
        void Set(const std::string name, float x, float y, float z, float w);
        void Set(const std::string name, glm::mat4 &value);
    protected:
        unsigned int m_Id;
    };

    class ShaderResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.shader.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };
}
