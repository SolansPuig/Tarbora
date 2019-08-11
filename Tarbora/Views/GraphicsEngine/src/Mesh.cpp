#include "../inc/GraphicsEngine.hpp"
#include "../../GraphicViews/inc/GraphicView.hpp"

#define GAMEVIEW(APP) static_cast<GraphicView*>(APP)

namespace Tarbora {
    Mesh::~Mesh()
    {
        GAMEVIEW(app)->GraphicsEngine()->DeleteMesh(m_Id);
    }

    ResourcePtr MeshResourceLoader::Load(std::string path)
    {
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        // Read the file into a vector
        std::string line;
        std::vector<float> v;
        int vertices = 0;
        while (std::getline(file, line))
        {
            float value;
            bool valid_line = false;
            std::stringstream ss(line);
            while (ss >> value)
            {
                v.push_back(value);
                valid_line = true;
            }
            if (valid_line) vertices++;
        }

        // Create the Mesh
        unsigned int id = GAMEVIEW(app)->GraphicsEngine()->LoadMesh(v);

        ResourcePtr r = ResourcePtr(new Mesh(app, path, id, vertices));
        return r;
    }
}
