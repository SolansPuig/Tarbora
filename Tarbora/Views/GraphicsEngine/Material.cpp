#include "GraphicsEngine.hpp"

namespace Tarbora {
    void Material::Bind()
    {
        m_Shader->Use();
        m_Albedo->Bind(0);
        m_Specular->Bind(1);
    }

    std::shared_ptr<Resource> MaterialResourceLoader::Load(std::string path)
    {
        LOG_DEBUG("Loading Material %s", path.c_str());
        // Open the Json containing all the info for that material
        std::ifstream file;
        file.open(path.c_str());
        if (file.fail())
            return std::shared_ptr<Resource>();

        raw_json j;
        try
        {
            j = raw_json::parse(file);
        }
        catch (raw_json::parse_error& e)
        {
            LOG_ERR("JsonResourceLoader: Trying to parse file \"%s\" found exception: \n \"%s\"", path.c_str(), e.what());
            file.close();
            return std::shared_ptr<Resource>();
        }

        // Create the material resource
        std::shared_ptr<Material> mat(new Material(m_Module, path));

        // Set its textures and shader
        if (j["albedo"].is_string())
        {
            std::string albedo = j["albedo"];
            mat->m_Albedo = (albedo != "") ?
                ResourcePtr<Texture>("textures/" + albedo) :
                ResourcePtr<Texture>("textures/white.png");
        }

        if (j["specular"].is_string())
        {
            std::string specular = j["specular"];
            mat->m_Specular = (specular != "") ?
                ResourcePtr<Texture>("textures/" + specular) :
                ResourcePtr<Texture>("textures/grey.png");
        }

        if (j["shader"].is_string() && j["shader"] != "")
        {
            std::string shader = j["shader"];
            mat->m_Shader = ResourcePtr<Shader>("shaders/" + shader);
            mat->m_Shader.SetInitialConfig([](auto shader){
                shader->Use();
                shader->Set("albedo", 0);
                shader->Set("specular", 1);
            });
        }

        file.close();
        return mat;
    }
}
