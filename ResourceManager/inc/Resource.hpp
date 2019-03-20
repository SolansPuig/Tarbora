#pragma once
#include <memory>
#include <string>
#include "ResourceManager.hpp"

namespace Tarbora {
    class ResourceLoader;

    class Resource
    {
        friend class ResourceLoader;
    public:
        Resource(const std::string name, unsigned int size=0) : m_Name(name), m_Size(size) {}
        ~Resource()
        {
            ResourceManager::MemoryHasBeenFreed(m_Size);
        }

        unsigned int GetSize() const { return m_Size; }
        std::string GetName() const { return m_Name; }

    protected:
        std::string m_Name;
        unsigned int m_Size;
    };

    typedef std::shared_ptr<Resource> ResourcePtr;

    class ResourceLoader
    {
    public:
        virtual std::string GetPattern() = 0;
        virtual ResourcePtr Load(std::string path) = 0;
    };

    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    class DefaultResourceLoader : public ResourceLoader
    {
    public:
        virtual std::string GetPattern() override { return "*"; };
        virtual ResourcePtr Load(std::string path) override { (void)(path); return ResourcePtr(); }
    };

    class TextResource : public Resource
    {
    public:
        TextResource(std::string name, unsigned int size, std::string textName ) : Resource(name, size)
        {
            m_Text.open(textName);
        }
        ~TextResource() { m_Text.close(); }

        std::fstream GetText() { return m_Text; }
    protected:
        std::fstream m_Text;
    }

    class TextResourceLoader : public ResourceLoader
    {
        virtual std::string GetPattern() override { return "*.txt"; };
        virtual ResourcePtr Load(std::string path) override {
            return ResourcePtr( new TextResource(path, ));
        }
    }
}
