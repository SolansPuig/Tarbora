#pragma once
#include "Component.hpp"

namespace Tarbora {
    class TypeComponent : public Component
    {
    public:
        TypeComponent(System *s, const ActorId &id, const LuaTable &table) :
            Component(s, id, table)
        {
            for (auto type : table)
            {
                Add(type.second.GetAs<std::string>());
            }
        }

        void Add(const std::string &type) { m_Types.push_back(type); }

        bool Has(const std::string &type) { return count(m_Types.begin(), m_Types.end(), type) > 0; }

    private:
        std::vector<std::string> m_Types;
    };

    class TypeSystem : public SystemImpl<TypeComponent>
    {
    public:
        TypeSystem(World *w) : SystemImpl<TypeComponent>(w) {}

        static std::string GetName() { return "type"; }
    };
}
