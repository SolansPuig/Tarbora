#pragma once
#include "ResourceManager.hpp"

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
}
