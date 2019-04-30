#include "../inc/Resource.hpp"

namespace Tarbora {
    ResourcePtr TextResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        std::string s = std::string (std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        ResourcePtr r = ResourcePtr(new Text(path, s));
        file.close();
        return r;
    }
}
