#include "SceneManager.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    void SceneManager::load(const std::string &level)
    {
        ResourcePtr<LuaScript> resource("levels/" + level);
        if (resource == nullptr) return;

        LOG_DEBUG("Loading level %s...", level.c_str());

        for (auto a : resource->get("actors"))
        {
            loadActor(a.second.getAs<LuaTable>());
        }
    }

    void SceneManager::loadActor(LuaTable actor)
    {
        getMessageManager()->send(1, "create_actor",
            Message::CreateActor(
                actor.get<std::string>("id", true),
                actor.get<std::string>("name"),
                actor.get<std::string>("variant", true),
                actor.get<glm::vec3>("position"),
                glm::quat(glm::radians(actor.get<glm::vec3>("rotation", true)))
            )
        );
    }
}
