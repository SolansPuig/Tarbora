#pragma once
#include "../../../Framework/Global.hpp"
#include "../../../Framework/ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Shader : public Resource
    {
        friend class ShaderResourceLoader;
    public:
        ~Shader();

        void Use() const;
        unsigned int GetId() const { return m_Id; }

        void Set(const std::string name, bool value);
        void Set(const std::string name, int value);
        void Set(const std::string name, float value);
        void Set(const std::string name, const glm::vec2 &value);
        void Set(const std::string name, float x, float y);
        void Set(const std::string name, const glm::vec3 &value);
        void Set(const std::string name, float x, float y, float z);
        void Set(const std::string name, const glm::vec4 &value);
        void Set(const std::string name, float x, float y, float z, float w);
        void Set(const std::string name, const glm::mat4 &value);
    private:
        Shader(std::string name) : Resource(name) {}
        void Delete();

        unsigned int CompileShader(const std::string &path, const std::string &type);
        unsigned int CompileShaderFile(const std::string &type, const std::string &code);
        void LinkProgram(unsigned int *ids);
        void DeleteProgram();

        unsigned int m_Id;
    };

    //! \cond HIDDEN_SYMBOLS
    class ShaderResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.shader.lua"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };
    //! \endcond
}
