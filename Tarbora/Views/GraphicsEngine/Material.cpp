#include "GraphicsEngine.hpp"
#include "../../Framework/ResourceManager/inc/Lua.hpp"

namespace Tarbora {
    void Material::Bind()
    {
        m_Shader->Use();
        m_Albedo->Bind(0);
        m_Specular->Bind(1);
        m_ColorTint->Bind(2);
    }

    std::shared_ptr<Resource> MaterialResourceLoader::Load(std::string path)
    {

        LuaScript resource(m_Module, path);
        // Create the material resource
        std::shared_ptr<Material> mat(new Material(m_Module, path));

        std::string albedo = resource.Get<std::string>("albedo");
        mat->m_Albedo = ResourcePtr<Texture>("textures/" + albedo);

        std::string specular = resource.Get<std::string>("specular", "grey.png", true);
        mat->m_Specular = ResourcePtr<Texture>("textures/" + specular);

        std::string colorTint = resource.Get<std::string>("colorTint", "generic_mask.png", true);
        mat->m_ColorTint = ResourcePtr<Texture>("textures/" + colorTint);

        std::string shader = resource.Get<std::string>("shader", "model.shader.json", true);
        mat->m_Shader = ResourcePtr<Shader>("shaders/" + shader);
        mat->m_Shader.SetInitialConfig([](auto shader){
            shader->Use();
            shader->Set("albedo", 0);
            shader->Set("specular", 1);
            shader->Set("colorTint", 2);
        });

        return mat;
    }
}
