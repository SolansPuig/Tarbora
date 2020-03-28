#include "ResourceManager.hpp"
#include "Resource.hpp"
#include "Lua.hpp"

namespace Tarbora {
    void ResourceManager::init(const std::string &folder_path)
    {
        folder_path_ = folder_path;

        // Register the default loaders.
        registerLoader(LoaderPtr(new DefaultResourceLoader()));
        registerLoader(LoaderPtr(new TextResourceLoader()));
        registerLoader(LoaderPtr(new LuaLoader()));
    }

    void ResourceManager::close()
    {
        flush();
    }

    void ResourceManager::registerLoader(std::shared_ptr<ResourceLoader> loader)
    {
        resource_loaders_.push_front(loader);
    }

    std::shared_ptr<Resource> ResourceManager::getResource(const std::string &name, bool *just_loaded)
    {
        // Try to find the resource, if not found, load it.
        std::shared_ptr<Resource> resource(findResource(name));
        if (!resource)
        {
            resource = loadResource(name);
            if (just_loaded != nullptr)
            {
                *just_loaded = true;
            }
        }

        return resource;
    }

    std::shared_ptr<Resource> ResourceManager::loadResource(const std::string &name)
    {
        ZoneScoped;

        // Find a loader that matches the resource's file name.
        std::shared_ptr<ResourceLoader> loader;
        for (auto resource_loader : resource_loaders_)
        {
            const std::string pattern = resource_loader->getPattern();
            if (pattern.length() < name.length())
            {
                if (0 == name.compare(name.length() - pattern.length(), pattern.length(), pattern))
                {
                    loader = resource_loader;
                    break;
                }
            }
        }

        // If no loader is found, that means that not even the default loaders are registered.
        // This should never happen.
        if (!loader)
        {
            LOG_ERR("ResourceManager: Default resource loader not found.");
            return std::shared_ptr<Resource>();
        }

        // Use the loader to load the resource.
        std::shared_ptr<Resource> resource = loader->load(folder_path_ + name);

        // If the resource gets loaded, store it. Else throw an error.
        if (resource)
        {
            resource_list_.push_front(resource);
            resource_map_[resource->getName()] = resource;
        } else {
            LOG_ERR("ResourceManager: Could not load resource \"%s\" with loader \"%s\".", name.c_str(), loader->getPattern().c_str());
        }

        // Return even if it's nullptr. It's callers responsibility to decide what to do.
        return resource;
    }

    std::shared_ptr<Resource> ResourceManager::findResource(const std::string &name)
    {
        auto itr = resource_map_.find(folder_path_ + name);
        if (itr == resource_map_.end())
            return std::shared_ptr<Resource>();
        return itr->second;
    }

    void ResourceManager::freeResource(std::shared_ptr<Resource> resource)
    {
        // Remove the resource from the list and the map. Don't destroy it as some
        // system may be still using it. As it's a shared pointer, it will eventually
        // get destroyed when no longer used.
        resource_list_.remove(resource);
        resource_map_.erase(resource->getName());
    }

    void ResourceManager::flush()
    {
        // Iterate the list freeing one by one all the resources.
    	while (!resource_list_.empty())
    	{
            auto gonner = resource_list_.end();
            gonner--;

            std::shared_ptr<Resource> resource = *gonner;
    		freeResource(resource);
    	}
    }
}
