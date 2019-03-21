#pragma once
#include <memory>
#include <string>
#include <fstream>
#include "ResourceManager.hpp"
#include "json.hpp"
#include <glm/glm.hpp>
typedef nlohmann::json json;

namespace Tarbora {
    class ResourceLoader;

    class Resource
    {
        friend class ResourceLoader;
    public:
        Resource(const std::string name) : m_Name(name) {}
        std::string GetName() const { return m_Name; }

    protected:
        std::string m_Name;
    };

    typedef std::shared_ptr<Resource> ResourcePtr;

    class ResourceLoader
    {
    public:
        virtual const std::string GetPattern() = 0;
        virtual ResourcePtr Load(std::string path) = 0;
    };

    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    class DefaultResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*"; }
        virtual ResourcePtr Load(std::string path) override { (void)(path); return ResourcePtr(); }
    };

    class TextResource : public Resource
    {
    public:
        TextResource(std::string name, std::string text) : Resource(name), m_Text(text) {}
        const std::string GetText() const { return m_Text; }
    protected:
        std::string m_Text;
    };

    class TextResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.txt"; };
        virtual ResourcePtr Load(std::string path) override;
    };

    class JsonResource : public Resource
    {
    public:
        JsonResource(std::string name, json j) : Resource(name), m_JSON(j) {}
        const json GetJson() const { return m_JSON; }
    protected:
        json m_JSON;
    };

    class JsonResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };

    class Shader : public Resource
    {
    public:
        Shader(std::string name, unsigned int id) : Resource(name), m_Id(id) {}
        ~Shader();
        void Use();
        void Set(const std::string &name, bool value);
        void Set(const std::string &name, int value);
        void Set(const std::string &name, float value);
        void Set(const std::string &name, glm::vec2 value);
        void Set(const std::string &name, float x, float y);
        void Set(const std::string &name, glm::vec3 value);
        void Set(const std::string &name, float x, float y, float z);
        void Set(const std::string &name, glm::vec4 value);
        void Set(const std::string &name, float x, float y, float z, float w);
        void Set(const std::string &name, glm::mat4 value);
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
