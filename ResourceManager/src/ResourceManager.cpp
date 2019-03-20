#include "ResourceManager.hpp"
#include <fnmatch.h>
#include <list>
#include <map>
#include "Logger.hpp"
#include "Resource.hpp"

namespace Tarbora {
    typedef std::list<ResourcePtr> ResourceList;
    typedef std::map<std::string, ResourcePtr> ResourceMap;
    typedef std::list<LoaderPtr> ResourceLoaders;

    ResourceList m_LRU; // least recently used list
    ResourceMap m_Resources;
    ResourceLoaders m_ResourceLoaders;

    std::string m_ResourceFolderPath;

    unsigned int m_CacheSize;
    unsigned int m_Allocated;

    namespace ResourceManager {
        ResourcePtr Find(const std::string resource);
        void Update(ResourcePtr resource);
        ResourcePtr Load(const std::string resource);
        bool MakeRoom(unsigned int size);
        void FreeOneResource();
    }

    void ResourceManager::Init(unsigned int sizeInMb, std::string resourceFolderPath)
    {
        m_CacheSize = sizeInMb * 1024 * 1024;
        m_Allocated = 0;
        m_ResourceFolderPath = resourceFolderPath;
        RegisterLoader(LoaderPtr(new DefaultResourceLoader()));
    }

    void ResourceManager::RegisterLoader(LoaderPtr loader)
    {
        m_ResourceLoaders.push_front(loader);
    }

    ResourcePtr ResourceManager::GetResource(const std::string resource)
    {
        ResourcePtr r(Find(resource));
        if (r == NULL)
            r = Load(resource);
        else
            Update(r);
        return r;
    }

    ResourcePtr ResourceManager::Load(const std::string resource)
    {
        LoaderPtr loader;
        for (auto itr = m_ResourceLoaders.begin(); itr != m_ResourceLoaders.end(); itr++)
        {
            LoaderPtr testLoader = *itr;
            if (fnmatch(testLoader->GetPattern().c_str(), resource.c_str(), FNM_CASEFOLD) == 0)
            {
                loader = testLoader;
                break;
            }
        }

        if (!loader)
        {
            LOG_ERR("ResourceManager: Default resource loader not found.");
            return ResourcePtr();
        }

        ResourcePtr r = loader->Load(m_ResourceFolderPath + resource);

        if (r)
        {
            m_LRU.push_front(r);
            m_Resources[r->GetName()] = r;
        } else {
            LOG_ERR("ResourceManager: Could not load resource %s.", resource);
        }

        return r;
    }

    ResourcePtr ResourceManager::Find(const std::string resource)
    {
        auto itr = m_Resources.find(resource);
        if (itr == m_Resources.end())
            return ResourcePtr();
        return itr->second;
    }

    void ResourceManager::Update(ResourcePtr resource)
    {
        m_LRU.remove(resource);
        m_LRU.push_front(resource);
    }

    void ResourceManager::Allocate(unsigned int size)
    {
        if (MakeRoom(size))
            m_Allocated += size;
    }

    bool ResourceManager::MakeRoom(unsigned int size)
    {
        if (size > m_CacheSize)
        {
            LOG_ERR("ResourceManager: Could not allocate %d bytes.", size);
            return false;
        }

        while (size > (m_CacheSize - m_Allocated))
        {
            if (m_LRU.empty())
            {
                LOG_ERR("ResourceManager: Could not allocate %d bytes.", size);
                return false;
            }

            FreeOneResource();
        }

        return true;
    }

    void ResourceManager::FreeOneResource()
    {
        auto gonner = m_LRU.end();
        gonner--;

        ResourcePtr r = *gonner;

        m_LRU.pop_back();
        m_Resources.erase(r->GetName());
    }

    void ResourceManager::Flush()
    {
    	while (!m_LRU.empty())
    	{
    		FreeOneResource();
    	}
    }

    void ResourceManager::MemoryHasBeenFreed(unsigned int size)
    {
        m_Allocated -= size;
    }
}
