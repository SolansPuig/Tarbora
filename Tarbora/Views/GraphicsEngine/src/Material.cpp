#include "../inc/GraphicsEngine.hpp"

namespace Tarbora {
    ResourcePtr MaterialResourceLoader::Load(std::string path)
    {
        LOG_DEBUG("Loading Material %s", path.c_str());
        // Open the Json containing all the info for that material
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return ResourcePtr();

        raw_json j;
        try
        {
            j = raw_json::parse(file);
        }
        catch (raw_json::parse_error& e)
        {
            LOG_ERR("JsonResourceLoader: Trying to parse file \"%s\" found exception: \n \"%s\"", path.c_str(), e.what());
            file.close();
            return ResourcePtr();
        }

        // Create the material resource
        std::shared_ptr<Material> mat(new Material(m_Module, path));

        // Set its texture and shader
        if (j["texture"].is_string() && j["texture"] != "")
        {
            std::string texture = j["texture"];
            mat->m_Texture = (texture != "") ?
                GET_RESOURCE(Texture, "textures/" + texture) :
                GET_RESOURCE(Texture, "textures/white.png");
            if (mat->m_Texture == nullptr)
                mat->m_Texture = GET_RESOURCE(Texture, "textures/missing.png");
        }

        if (j["shader"].is_string() && j["shader"] != "")
        {
            std::string shader = j["shader"];
            mat->m_Shader = GET_RESOURCE(Shader, "shaders/" + shader);
        }

        file.close();
        return mat;
    }
}
