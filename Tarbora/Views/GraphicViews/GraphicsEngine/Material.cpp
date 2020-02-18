#include "GraphicsEngine.hpp"
#include "../../../Framework/ResourceManager/Lua.hpp"

namespace Tarbora {
    void Material::bind(const glm::mat4 &projection, const glm::mat4 &view) const
    {
        shader_->use();
        shader_->set("pixelDensity", pixel_density_);
        shader_->set("projection", projection);
        shader_->set("view", view);
        albedo_->bind(0);
        specular_->bind(1);
        color_tint_->bind(2);
    }

    std::shared_ptr<Resource> MaterialResourceLoader::load(const std::string &path)
    {
        ZoneScoped;

        std::string albedo = "missing.png";
        std::string specular = "grey.png";
        std::string color_tint = "generic_mask.png";
        std::string shader = "model.shader.lua";
        int pixel_density = 100;

        {
            std::ifstream file(path);
            if (file) {
                LuaScript resource(path);
                albedo = resource.get<std::string>("albedo");
                specular = resource.get<std::string>("specular", specular, true);
                color_tint = resource.get<std::string>("color_tint", color_tint, true);
                shader = resource.get<std::string>("shader", shader, true);
                pixel_density = resource.get<int>("pixel_density", pixel_density, true);
            }
        }

        // Create the material resource
        std::shared_ptr<Material> mat(new Material(path));
        mat->albedo_ = ResourcePtr<Texture>("textures/" + albedo, "textures/missing.png");
        mat->specular_ = ResourcePtr<Texture>("textures/" + specular, "textures/grey.png");
        mat->color_tint_ = ResourcePtr<Texture>("textures/" + color_tint, "textures/generic_mask.png");
        mat->pixel_density_ = pixel_density;
        mat->shader_ = ResourcePtr<Shader>("shaders/" + shader, "shaders/model.shader.lua");
        mat->shader_.setInitialConfig([](auto shader){
            shader->use();
            shader->set("albedo", 0);
            shader->set("specular", 1);
            shader->set("colorTint", 2);
        });

        return mat;
    }
}
