#include "SceneManager.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    void SceneManager::Load(const std::string &level)
    {
        ResourcePtr<LuaScript> resource("levels/" + level);
        if (resource == nullptr) return;

        LOG_DEBUG("Loading level %s...", level.c_str());

        for (auto a : resource->Get("actors"))
        {
            LoadActor(a.second.GetAs<LuaTable>());
        }
    }

    void SceneManager::LoadActor(LuaTable actor)
    {
        GetMessageManager()->Send(1, "create_actor",
            CreateActor(
                actor.Get<std::string>("id", true),
                actor.Get<std::string>("name"),
                actor.Get<std::string>("variant", true),
                actor.Get<glm::vec3>("position"),
                actor.Get<glm::vec3>("rotation", true)
            )
        );
    }
}
