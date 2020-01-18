#pragma once
#include "../../Framework/Module.hpp"

namespace Tarbora {
    class ActorModel;

    class AnimationController
    {
    public:
        AnimationController(ActorModel *actor, const std::string &file);

        void Update(float deltaTime);

        void SetAnimation(const std::string &name);

    private:
        void UpdateAnimation();
        glm::vec3 ReadValueWithScale(LuaTable &p, LuaTable &value, const glm::vec3 &currentValue, float scale);
        glm::vec3 ReadValue(LuaTable &p, const std::string &name, LuaTable &value, const glm::vec3 &currentValue);

        ActorModel *m_ActorModel;
        std::string m_AnimationsFile;

        float m_Counter;
        float m_Crono;

        std::string m_CurrentAnimationName;
    };
}
