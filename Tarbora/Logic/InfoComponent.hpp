#pragma once
#include "Component.hpp"

namespace Tarbora {
    class InfoComponent : public Component
    {
    public:
        InfoComponent(System *s, const ActorId &id, const LuaTable &table) :
            Component(s, id, table)
        {
            entity_ = table.get<std::string>("entity");
            variant_ = table.get<std::string>("variant");
        }

        const std::string& getEntity() { return entity_; }
        const std::string& getVariant() { return variant_; }

    private:
        std::string entity_;
        std::string variant_;
    };

    class InfoSystem : public SystemImpl<InfoComponent>
    {
    public:
        InfoSystem(World *w) : SystemImpl<InfoComponent>(w) {}

        static std::string getName() { return "info"; }
    };
}
