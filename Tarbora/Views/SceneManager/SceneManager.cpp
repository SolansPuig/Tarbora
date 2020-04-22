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
        Message::CreateActor msg(
            actor.get<std::string>("id", true),
            actor.get<std::string>("name")
        );
        msg.setVariant(actor.get<std::string>("variant", true));
        msg.setPosition(actor.get<glm::vec3>("position"));
        msg.setOrientation(glm::radians(actor.get<glm::vec3>("rotation", true)));
        getMessageManager()->send(1, "create_actor", msg);
    }
}
