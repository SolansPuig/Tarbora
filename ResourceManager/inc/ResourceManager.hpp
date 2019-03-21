#pragma once
#include <memory>
#include <string>

namespace Tarbora {
    class Resource;
    class ResourceLoader;
    typedef std::shared_ptr<Resource> ResourcePtr;
    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    namespace ResourceManager
    {
        void Init(const std::string resourceFolderPath);
        void RegisterLoader(LoaderPtr);

        ResourcePtr Get(const std::string resource);
        // TODO: int Preload(const std::string pattern);
        void Flush(void);
        void Free(ResourcePtr resource);

        std::string GetResourceFolder();
    };
}

#define GET_RESOURCE(TYPE, NAME) std::static_pointer_cast<TYPE>(::Tarbora::ResourceManager::Get(NAME))
