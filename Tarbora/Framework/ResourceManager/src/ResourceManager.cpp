#include "../inc/Resource.hpp"
#include <fnmatch.h>

namespace Tarbora {
    typedef std::list<ResourcePtr> ResourceList;
    typedef std::map<std::string, ResourcePtr> ResourceMap;
    typedef std::list<LoaderPtr> ResourceLoaders;

    ResourceList m_List;
    ResourceMap m_Resources;
    ResourceLoaders m_ResourceLoaders;

    std::string m_ResourceFolderPath;

    namespace ResourceManager {
        ResourcePtr Find(const std::string resource);
        ResourcePtr Load(const std::string resource);
        void Free(ResourcePtr resource);
    }

    void ResourceManager::Init(std::string resourceFolderPath)
    {
        m_ResourceFolderPath = resourceFolderPath;
        RegisterLoader(LoaderPtr(new DefaultResourceLoader()));
        RegisterLoader(LoaderPtr(new TextResourceLoader()));
        RegisterLoader(LoaderPtr(new JsonResourceLoader()));
    }

    void ResourceManager::RegisterLoader(LoaderPtr loader)
    {
        m_ResourceLoaders.push_front(loader);
    }

    ResourcePtr ResourceManager::Get(const std::string resource)
    {
        ResourcePtr r(Find(resource));
        if (r == NULL)
            r = Load(resource);
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
            m_List.push_front(r);
            m_Resources[r->GetName()] = r;
        } else {
            LOG_ERR("ResourceManager: Could not load resource %s with loader %s.", resource.c_str(), loader->GetPattern().c_str());
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

    void ResourceManager::Free(ResourcePtr r)
    {
        m_List.remove(r);
        m_Resources.erase(r->GetName());
    }

    void ResourceManager::Flush()
    {
    	while (!m_List.empty())
    	{
            auto gonner = m_List.end();
            gonner--;

            ResourcePtr r = *gonner;
    		Free(r);
    	}
    }

    std::string ResourceManager::GetResourceFolder() { return m_ResourceFolderPath; }
}
