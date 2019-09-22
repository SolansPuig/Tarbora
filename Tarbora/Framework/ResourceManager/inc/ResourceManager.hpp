#pragma once
#include "../../CommonHeaders.hpp"
#include "../../Global.hpp"

namespace Tarbora {
    class Module;
    class Resource;
    class ResourceLoader;
    typedef std::shared_ptr<Resource> ResourcePtr;
    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    typedef std::list<ResourcePtr> ResourceList;
    typedef std::map<std::string, ResourcePtr> ResourceMap;
    typedef std::list<LoaderPtr> ResourceLoaders;

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
        static void Init(Module *m, const std::string resourceFolderPath);

        static void Close();

        //! Register a \a ResourceLoader.
        /*!
            \param  loader A pointer to the loader. See \ref Resource.
        */
        static void RegisterLoader(LoaderPtr loader);

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
        static ResourcePtr GetResource(const std::string resource);
        // TODO: int Preload(const std::string pattern);

        //! Destroy a \ref Resource.
        /*!
            \param resource The file name of the resource to destroy.
        */
        static void FreeResource(ResourcePtr resource);

        //! Destroy all the \ref Resources.
        static void Flush(void);

        //! Get the path to folder where all the resource files are be located at.
        static std::string GetResourceFolder() { return m_ResourceFolderPath; }

        inline static Module *m_Module;

    private:
        //! Find a resource that is already loaded.
        /*!
            \param resource The file name of the resource to find.
            \return A pointer to the \ref Resource or nullptr if not found.
        */
        static ResourcePtr FindResource(const std::string resource);

        //! Load a resource from a file.
        /*!
            \param resource The file name of the resource to load.
            \return A pointer to the \ref Resource or nullptr if can't load.
        */
        static ResourcePtr LoadResource(const std::string resource);

        inline static std::list<ResourcePtr> m_List; // List of all the loaded resources.
        inline static std::map<std::string, ResourcePtr> m_Resources; // Map from file name to resource pointer.
        inline static std::list<LoaderPtr> m_ResourceLoaders; // List of all the registered Resource Loaders, sorted by the most specidic first.
        inline static std::string m_ResourceFolderPath; // The path to folder where all the resource files are be located at.
    };

    #define GET_RESOURCE(TYPE, NAME) std::static_pointer_cast<TYPE>(::Tarbora::ResourceManager::GetResource(NAME))
}
