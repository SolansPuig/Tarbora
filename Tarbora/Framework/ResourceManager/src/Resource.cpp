#include "../inc/Resource.hpp"

namespace Tarbora {
    ResourcePtr TextResourceLoader::Load(std::string path)
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
}
