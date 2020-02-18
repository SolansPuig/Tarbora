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
                add(type.second.getAs<std::string>());
            }
        }

        void add(const std::string &type) { types_.push_back(type); }

        bool has(const std::string &type) { return count(types_.begin(), types_.end(), type) > 0; }

    private:
        std::vector<std::string> types_;
    };

    class TypeSystem : public SystemImpl<TypeComponent>
    {
    public:
        TypeSystem(World *w) : SystemImpl<TypeComponent>(w) {}

        static std::string getName() { return "type"; }
    };
}
