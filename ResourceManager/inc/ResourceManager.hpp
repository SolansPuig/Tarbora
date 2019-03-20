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
        void Init(unsigned int sizeInMb, const std::string resourceFolderPath);
        void RegisterLoader(LoaderPtr);

        ResourcePtr GetResource(const std::string resource);
        // TODO: int Preload(const std::string pattern);
        void Allocate(unsigned int size);
        void MemoryHasBeenFreed(unsigned int size);
        void Flush(void);
    };
}
