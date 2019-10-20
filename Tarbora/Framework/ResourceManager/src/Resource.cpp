#include "../inc/Resource.hpp"

namespace Tarbora {
    std::shared_ptr<Resource> TextResourceLoader::Load(std::string path)
    {
        // Try to open the file.
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return std::shared_ptr<Resource>();

        // Parse the contents to a string.
        std::string s = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        std::shared_ptr<Resource> r = std::shared_ptr<Resource>(new Text(m_Module, path, s)); // Create the Resource.
        file.close(); // Remember to close the file!
        return r;
    }
}
