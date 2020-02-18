#pragma once
#include "../../../Framework/Module/Module.hpp"
#include "../../Framework/Global.hpp"

namespace Tarbora {
    class SceneManager : public Module
    {
    public:
        SceneManager() : Module(2) {}
        ~SceneManager() {}

        void load(const std::string &level);
        virtual void update(float delta_time) override { UNUSED(delta_time); }

        void loadActor(LuaTable table);
    };
}
