#pragma once
#include "../../../Framework/Module.hpp"
#include "../../Framework/Global.hpp"

namespace Tarbora {
    class SceneManager : public Module
    {
    public:
        SceneManager() : Module(2) {}
        ~SceneManager() {}

        void Load(const std::string &level);
        virtual void Update(float elapsed_time) override {}

        void LoadActor(LuaTable table);
    };
}
