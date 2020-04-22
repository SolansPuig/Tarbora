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
            virtual const std::string getPattern();
            virtual std::shared_ptr<Resource> load(std::string path);
        \endcode

        \a getPattern returns a regex, the files that match it will be loaded with that loader.

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
                virtual const std::string getPattern() override { return "*.txt"; }
                virtual std::shared_ptr<Resource> load(std::string path) override
                {
                    // Try to open the file.
                    std::ifstream file;
                    file.open(path.c_str());
                    if (file.fail())
                        return std::shared_ptr<Resource>();

                    // Parse the contents to a string.
                    std::string s = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                    std::shared_ptr<Resource> r = std::shared_ptr<Resource>(new Text(path, s)); // Create the Resource.
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
        const std::string& getName() const { return name_; }

    protected:
        Resource(const std::string &name) : name_(name) {}

        //! The filename of the resource.
        std::string name_;
    };

    //! \cond HIDDEN_SYMBOLS
    class ResourceLoader
    {
        friend class ResourceManager;

    private:
        virtual const std::string getPattern() = 0;
        virtual std::shared_ptr<Resource> load(const std::string &path) = 0;
    };
    //! \endcond

    typedef std::shared_ptr<ResourceLoader> LoaderPtr;

    //! \cond HIDDEN_SYMBOLS
    class DefaultResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string getPattern() override { return ""; }
        virtual std::shared_ptr<Resource> load(const std::string &path) override { (void)(path); return std::shared_ptr<Resource>(); }
    };
    //! \endcond

    //! A wrapper class for a raw text file.
    class Text : public Resource
    {
        friend class TextResourceLoader;
    public:
        //! Returns an string containing all the content of the file.
        const std::string& getText() const { return text_; }
    private:
        Text(const std::string &name, const std::string &text) : Resource(name), text_(text) {}

        std::string text_;
    };

    //! \cond HIDDEN_SYMBOLS
    class TextResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string getPattern() override { return ".txt"; }
        virtual std::shared_ptr<Resource> load(const std::string &path) override;
    };
    //! \endcond

    template <class T>
    class ResourcePtr
    {
    public:
        ResourcePtr(const ResourcePtr &r) = default;
        ResourcePtr() : initial_config_(nullptr) {}
        ResourcePtr(const std::string &name, const std::string &fallback_name="")
            : name_(name), fallback_name_(fallback_name), initial_config_(nullptr) {}

        const std::string& getName() const { return name_; }

        void setInitialConfig(const std::function<void(std::shared_ptr<T>)> &fn)
        {
            initial_config_ = fn;
        }

        bool operator==(std::nullptr_t) const
        {
            return (name_ == "") || ResourceManager::getResource(name_) == nullptr;
        }

        bool operator!=(std::nullptr_t) const
        {
            return !((name_ == "") || ResourceManager::getResource(name_) == nullptr);
        }

        bool operator==(const ResourcePtr<T> &resource) const
        {
            return name_ == resource.name_;
        }

        bool operator!=(const ResourcePtr<T> &resource) const
        {
            return !(*this == resource);
        }

        bool operator>(const ResourcePtr<T> &resource) const
        {
            return name_ > resource.name_;
        }

        ResourcePtr<T>& operator=(const ResourcePtr<T> &resource)
        {
            if (this != &resource) // Avoid self assignment
            {
                name_ = resource.name_;
                fallback_name_ = resource.fallback_name_;
                initial_config_ = resource.initial_config_;
            }
            return *this;
        }

        std::shared_ptr<T> operator->() const
        {
            bool just_loaded = false;
            std::shared_ptr<T> resource = std::static_pointer_cast<T>(ResourceManager::getResource(name_, &just_loaded));
            if (resource == nullptr && fallback_name_ != "")
                resource = std::static_pointer_cast<T>(ResourceManager::getResource(fallback_name_, &just_loaded));
            if (just_loaded && initial_config_ != nullptr) initial_config_(resource);
            return resource;
        }

        T& operator*() const
        {
            bool just_loaded = false;
            std::shared_ptr<T> resource = std::static_pointer_cast<T>(ResourceManager::getResource(name_, &just_loaded));
            if (resource == nullptr && fallback_name_ != "")
                resource = std::static_pointer_cast<T>(ResourceManager::getResource(fallback_name_, &just_loaded));
            if (just_loaded && initial_config_ != nullptr) initial_config_(resource);
            return *resource;
        }
    private:
        std::string name_;
        std::string fallback_name_;
        std::function<void(std::shared_ptr<T>)> initial_config_;
    };

    class ResourcePtrHash
    {
    public:
        template <class T>
        std::size_t operator()(const ResourcePtr<T>& p) const
        {
            return std::hash<std::string>()(p->getName());
        }
    };
}
