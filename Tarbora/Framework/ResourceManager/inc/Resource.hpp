#pragma once
#include "ResourceManager.hpp"

namespace Tarbora {
    class ResourceLoader;

    //! An abstract resource.
    /*!
        This resource will be shared between all the classes that use it, and
        loaded only once (probably in startup or level load).

        If you want to implement your own resource, inherit from this class.
        Make sure your constructor calls the \ref Resource constructor with the
        filename of the resource.

        A resource constructor should be private, as it should only be created
        inside a \ref ResourceLoader.

        \see ResourceLoader
        \see Json
        \see Text
    */
    class Resource
    {
        friend class ResourceLoader;
    public:
        //! Returns the filename of the resource.
        std::string GetName() const { return m_Name; }

    protected:
        Resource(const std::string name) : m_Name(name) {}

        //! The filename of the resource.
        std::string m_Name;
    };

    typedef std::shared_ptr<Resource> ResourcePtr;

    //! An abstract resource loader, used to load a \ref Resource from file.
    /*!
        This will be called only once for every resource (probably in startup or level load).

        If you want to implement your own resource, create also a ResourceLoader
        inherited from this class.

        You must register the resource loader to the \ref ResourceManager

        \see Resource
    */
    class ResourceLoader
    {
    public:
        //! Returns the pattern that will be used to select a suitable loader for a file, a regex.
        virtual const std::string GetPattern() = 0;

        //! Load the file located in \a path. Returns a nullptr if not found or if any error occurs.
        virtual ResourcePtr Load(std::string path) = 0;
    };

    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    //! \cond HIDDEN_SYMBOLS
    class DefaultResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*"; }
        virtual ResourcePtr Load(std::string path) override { (void)(path); return ResourcePtr(); }
    };
    //! \endcond

    //! Wrapper class for a raw text file.
    class Text : public Resource
    {
        friend class TextResourceLoader;
    public:
        //! Returns an string containing all the content of the file.
        const std::string GetText() const { return m_Text; }
    private:
        Text(std::string name, std::string text) : Resource(name), m_Text(text) {}

        std::string m_Text;
    };

    //! \cond HIDDEN_SYMBOLS
    class TextResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.txt"; }
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
