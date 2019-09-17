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

        A resource constructor must be private, as it can only be created
        inside a \a ResourceLoader.

        A ResourceLoader is also needed, it must inherit from \a ResourceLoader,
        be a friend of the class \ref ResourceManager and implement two private methods:

        \code{.cpp}
            virtual const std::string GetPattern();
            virtual ResourcePtr Load(std::string path);
        \endcode

        \a GetPattern returns a regex, the files that match it will be loaded with that loader.

        \a Load loads the file in \a path, reading it from disk and parsing it or doing the needed
        conversions.

        All ResourceLoaders need to be registered in \ref ResourceManager to work.

        If a file would match the pattern of several ResourceLoaders, it will be loaded by
        the lastest registered one.

        Here's an example of the implementation of a Resource and its \a ResourceLoader for txt
        files:

        \code{.cpp}
            class Text : public Resource
            {
                friend class TextResourceLoader;
            public:
                const std::string GetText() const { return m_Text; }
            private:
                Text(std::string name, std::string text) : Resource(name), m_Text(text) {}

                std::string m_Text;
            };

            class TextResourceLoader : public ResourceLoader
            {
                friend class ResourceManager;
            private:
                virtual const std::string GetPattern() override { return "*.txt"; }
                virtual ResourcePtr Load(std::string path) override
                {
                    // Try to open the file.
                    std::ifstream file;
                    file.open(path.c_str());
                    if (file.fail())
                        return ResourcePtr();

                    // Parse the contents to a string.
                    std::string s = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                    ResourcePtr r = ResourcePtr(new Text(path, s)); // Create the Resource.
                    file.close(); // Remember to close the file!
                    return r;
                }
            };
        \endcode

        \see ResourceManager
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
        Resource(Module *m, const std::string name) : m_Module(m), m_Name(name) {}

        Module *m_Module;

        //! The filename of the resource.
        std::string m_Name;
    };

    typedef std::shared_ptr<Resource> ResourcePtr;

    //! \cond HIDDEN_SYMBOLS
    class ResourceLoader
    {
        friend class ResourceManager;

    protected:
        Module *m_Module;

    private:
        virtual const std::string GetPattern() = 0;
        virtual ResourcePtr Load(std::string path) = 0;
    };
    //! \endcond

    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    //! \cond HIDDEN_SYMBOLS
    class DefaultResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*"; }
        virtual ResourcePtr Load(std::string path) override { (void)(path); return ResourcePtr(); }
    };
    //! \endcond

    //! A wrapper class for a raw text file.
    class Text : public Resource
    {
        friend class TextResourceLoader;
    public:
        //! Returns an string containing all the content of the file.
        const std::string GetText() const { return m_Text; }
    private:
        Text(Module *m, std::string name, std::string text) : Resource(m_Module, name), m_Text(text) {}

        std::string m_Text;
    };

    //! \cond HIDDEN_SYMBOLS
    class TextResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.txt"; }
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
