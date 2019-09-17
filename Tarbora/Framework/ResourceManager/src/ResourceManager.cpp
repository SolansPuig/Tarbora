#include "../inc/ResourceManager.hpp"
#include "../inc/Resource.hpp"
#include "../inc/Json.hpp"
#include <fnmatch.h>

namespace Tarbora {
    void ResourceManager::Init(Module *m, const std::string resourceFolderPath)
    {
        m_Module = m;
        m_ResourceFolderPath = resourceFolderPath;

        // Register the default loaders.
        RegisterLoader(LoaderPtr(new DefaultResourceLoader()));
        RegisterLoader(LoaderPtr(new TextResourceLoader()));
        RegisterLoader(LoaderPtr(new JsonResourceLoader()));
    }

    void ResourceManager::Close()
    {

    }

    void ResourceManager::RegisterLoader(LoaderPtr loader)
    {
        loader->m_Module = m_Module;
        m_ResourceLoaders.push_front(loader);
    }

    ResourcePtr ResourceManager::GetResource(const std::string resource)
    {
        // Try to find the resource, if not found, load it.
        ResourcePtr r(FindResource(resource));
        if (!r)
            r = LoadResource(resource);
        return r;
    }

    ResourcePtr ResourceManager::LoadResource(const std::string resource)
    {
        // Find a loader that matches the resource's file name.
        LoaderPtr loader;
        for (auto itr = m_ResourceLoaders.begin(); itr != m_ResourceLoaders.end(); itr++)
        {
            LoaderPtr testLoader = *itr;
            // This will not work on windows.
            if (fnmatch(testLoader->GetPattern().c_str(), resource.c_str(), FNM_CASEFOLD) == 0)
            {
                loader = testLoader;
                break;
            }
        }

        // If no loader is found, that means that not event the default loaders are registered.
        // This should never happen.
        if (!loader)
        {
            LOG_ERR("ResourceManager: Default resource loader not found.");
            return ResourcePtr();
        }

        // Use the loader to load the resource.
        ResourcePtr r = loader->Load(m_ResourceFolderPath + resource);

        // If the resource gets loaded, store it. Else throw an error.
        if (r)
        {
            m_List.push_front(r);
            m_Resources[r->GetName()] = r;
        } else {
            LOG_ERR("ResourceManager: Could not load resource \"%s\" with loader \"%s\".", resource.c_str(), loader->GetPattern().c_str());
        }

        // Return even if it's nullptr. It's callers responsibility to decide what to do.
        return r;
    }

    ResourcePtr ResourceManager::FindResource(const std::string resource)
    {
        auto itr = m_Resources.find(m_ResourceFolderPath + resource);
        if (itr == m_Resources.end())
            return ResourcePtr();
        return itr->second;
    }

    void ResourceManager::FreeResource(ResourcePtr r)
    {
        // Remove the resource from the list and the map. Don't destroy it as some
        // system may be still using it. As it's a shared pointer, it will eventually
        // get destroyed when no longer used.
        m_List.remove(r);
        m_Resources.erase(r->GetName());
    }

    void ResourceManager::Flush()
    {
        // Iterate the list freeing one by one all the resources.
    	while (!m_List.empty())
    	{
            auto gonner = m_List.end();
            gonner--;

            ResourcePtr r = *gonner;
    		FreeResource(r);
    	}
    }
}
