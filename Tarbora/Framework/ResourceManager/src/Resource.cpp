#include "../inc/Resource.hpp"
#define STB_IMAGE_IMPLEMENTATION

namespace Tarbora {
    ResourcePtr TextResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        std::string s = std::string (std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        ResourcePtr r = ResourcePtr(new TextResource(path, s));
        file.close();
        return r;
    }

    ResourcePtr JsonResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();
        ResourcePtr r = ResourcePtr(new JsonResource(path, json::parse(file)));
        file.close();
        return r;
    }
}
