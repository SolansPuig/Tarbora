#pragma once
#include "../../CommonHeaders.hpp"
#include "../../Global.hpp"

namespace Tarbora {
    class Resource;
    class ResourceLoader;

    //! Load resources and make them available to all the systems.
    /*!
        See an example of usage in \ref Json.
        \see Resource
    */
    class ResourceManager
    {
    public:
        //! Start the ResourceManager.
        /*!
            \param resourceFolderPath The path to folder where all the resource files are be located at.
            It must be called on startup, before initializing any system that uses resources.
        */
        static void init(const std::string &folder_path);

        static void close();

        //! Register a \a ResourceLoader.
        /*!
            \param  loader A pointer to the loader. See \ref Resource.
        */
        static void registerLoader(std::shared_ptr<ResourceLoader>);

        //! Get a \ref Resource by name.
        /*!
            \param  resource The file name of the resource.
            \return A pointer to a \ref Resource.

            The returned \ref Resource must be casted to the required type, so
            a macro is provided to make Get easier, as it typecasts the result:
            \code{.cpp}
                GET_RESOURCE(TYPE, NAME)
            \endcode

            If the \ref Resource isn't loaded yet, \ref GetResource automatically finds the
            appropiate \a ResourceLoader (see \ref Resource) and loads it.
        */
        static std::shared_ptr<Resource> getResource(const std::string &resource, bool *just_loaded=nullptr);
        // TODO: int Preload(const std::string pattern);

        //! Destroy a \ref Resource.
        /*!
            \param resource The file name of the resource to destroy.
        */
        static void freeResource(std::shared_ptr<Resource> resource);

        //! Destroy all the \ref Resources.
        static void flush();

        //! Get the path to folder where all the resource files are be located at.
        static std::string &getResourceFolder() { return folder_path_; }

    private:
        //! Find a resource that is already loaded.
        /*!
            \param resource The file name of the resource to find.
            \return A pointer to the \ref Resource or nullptr if not found.
        */
        static std::shared_ptr<Resource> findResource(const std::string &resource);

        //! Load a resource from a file.
        /*!
            \param resource The file name of the resource to load.
            \return A pointer to the \ref Resource or nullptr if can't load.
        */
        static std::shared_ptr<Resource> loadResource(const std::string &resource);

        inline static std::list<std::shared_ptr<Resource>> resource_list_; // List of all the loaded resources.
        inline static std::map<std::string, std::shared_ptr<Resource>> resource_map_; // Map from file name to resource pointer.
        inline static std::list<std::shared_ptr<ResourceLoader>> resource_loaders_; // List of all the registered Resource Loaders, sorted by the most specidic first.
        inline static std::string folder_path_; // The path to folder where all the resource files are be located at.
    };

    #define GET_RESOURCE(TYPE, NAME) std::static_pointer_cast<TYPE>(::Tarbora::ResourceManager::getResource(NAME))
}
