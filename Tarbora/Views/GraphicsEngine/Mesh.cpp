#include "GraphicsEngine.hpp"
#include "../../Framework/Module.hpp"

namespace Tarbora {
    std::shared_ptr<Resource> MeshResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return std::shared_ptr<Resource>();

        // Read the file into a vector
        std::string line;
        std::vector<float> data;
        unsigned int vertices = 0;
        while (std::getline(file, line))
        {
            float value;
            bool validLine = false;
            std::stringstream ss(line);
            while (ss >> value)
            {
                data.push_back(value);
                validLine = true;
            }
            if (validLine) vertices++;
        }

        std::shared_ptr<Resource> r = std::shared_ptr<Resource>(new Mesh(m_Module, path, data, vertices));
        return r;
    }
}
