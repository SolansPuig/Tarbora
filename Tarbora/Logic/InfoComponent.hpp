#pragma once
#include "Component.hpp"

namespace Tarbora {
    class InfoComponent : public Component
    {
    public:
        InfoComponent(System *s, const ActorId &id, const LuaTable &table) :
            Component(s, id, table)
        {
            m_Entity = table.Get<std::string>("entity");
            m_Variant = table.Get<std::string>("variant");
        }

        const std::string &GetEntity() { return m_Entity; }
        const std::string &GetVariant() { return m_Variant; }

    private:
        std::string m_Entity;
        std::string m_Variant;
    };

    class InfoSystem : public SystemImpl<InfoComponent>
    {
    public:
        InfoSystem(World *w) : SystemImpl<InfoComponent>(w) {}

        static std::string GetName() { return "info"; }
    };
}
