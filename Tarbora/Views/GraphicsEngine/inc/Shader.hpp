#pragma once
#include "../../../Framework/Global.hpp"
#include "../../../Framework/ResourceManager/inc/Resource.hpp"
#include "../../../Framework/External/json.hpp"
typedef nlohmann::json raw_json;

namespace Tarbora {
    class Shader : public Resource
    {
        friend class ShaderResourceLoader;
    public:
        ~Shader();

        void Use();
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
        Shader(Module *m, std::string name);
        void Delete();

        unsigned int CompileShader(raw_json j, const char* type);
        unsigned int CompileShaderFile(std::string type, const char *code);
        void LinkProgram(unsigned int *ids);
        void DeleteProgram();

        unsigned int m_Id;
    };

    //! \cond HIDDEN_SYMBOLS
    class ShaderResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.shader.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
