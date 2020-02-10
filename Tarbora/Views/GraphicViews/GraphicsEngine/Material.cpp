#include "GraphicsEngine.hpp"
#include "../../../Framework/ResourceManager/inc/Lua.hpp"

namespace Tarbora {
    void Material::Bind(const glm::mat4 &projection, const glm::mat4 &view) const
    {
        m_Shader->Use();
        m_Shader->Set("pixelDensity", m_PixelDensity);
        m_Shader->Set("projection", projection);
        m_Shader->Set("view", view);
        m_Albedo->Bind(0);
        m_Specular->Bind(1);
        m_ColorTint->Bind(2);
    }

    std::shared_ptr<Resource> MaterialResourceLoader::Load(std::string path)
    {
        std::string albedo = "missing.png";
        std::string specular = "grey.png";
        std::string colorTint = "generic_mask.png";
        std::string shader = "model.shader.lua";
        int pixelDensity = 100;

        {
            std::ifstream file(path);
            if (file) {
                LuaScript resource(path);
                albedo = resource.Get<std::string>("albedo");
                specular = resource.Get<std::string>("specular", specular, true);
                colorTint = resource.Get<std::string>("colorTint", colorTint, true);
                shader = resource.Get<std::string>("shader", shader, true);
                pixelDensity = resource.Get<int>("pixel_density", pixelDensity, true);
            }
        }

        // Create the material resource
        std::shared_ptr<Material> mat(new Material(path));
        mat->m_Albedo = ResourcePtr<Texture>("textures/" + albedo, "textures/missing.png");
        mat->m_Specular = ResourcePtr<Texture>("textures/" + specular, "textures/grey.png");
        mat->m_ColorTint = ResourcePtr<Texture>("textures/" + colorTint, "textures/generic_mask.png");
        mat->m_PixelDensity = pixelDensity;
        mat->m_Shader = ResourcePtr<Shader>("shaders/" + shader, "shaders/model.shader.lua");
        mat->m_Shader.SetInitialConfig([](auto shader){
            shader->Use();
            shader->Set("albedo", 0);
            shader->Set("specular", 1);
            shader->Set("colorTint", 2);
        });

        return mat;
    }
}
