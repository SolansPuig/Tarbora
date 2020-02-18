#include "Resource.hpp"

namespace Tarbora {
    std::shared_ptr<Resource> TextResourceLoader::load(const std::string &path)
    {
        std::shared_ptr<Resource> r;
        {
            // Try to open the file.
            std::ifstream file(path.c_str());
            if (!file.fail())
            {
                // Parse the contents to a string.
                const std::string s = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                r = std::shared_ptr<Resource>(new Text(path, s));
            }
        }

        return r;
    }
}
