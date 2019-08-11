#pragma once

#include "MathMessages.hpp"
#include "../proto/basic.pb.h"

using namespace tbBasics;

namespace Tarbora {
    class ClientApplication;

    void CreateActor(ClientApplication *app, std::string entity, std::string variant, std::string position, std::string rotation="0,0,0");
    void CreateActor(ClientApplication *app, std::string entity, std::string variant, glm::vec3 position, glm::vec3 rotation=glm::vec3(0.f,0.f,0.f));

    void CreateActorModel(ClientApplication *app, unsigned int id, std::string entity, std::string variant);

    void DeleteActor(ClientApplication *app, unsigned int id);

    void MoveActor(ClientApplication *app, unsigned int id, std::string position, std::string rotation);
    void MoveActor(ClientApplication *app, unsigned int id, glm::vec3 position, glm::mat3 rotation);

    void AnimateActor(ClientApplication *app, unsigned int id, std::string animation);

    void ApplyForce(ClientApplication *app, unsigned int id, float magnitude, std::string direction);
    void ApplyForce(ClientApplication *app, unsigned int id, float magnitude, glm::vec3 direction);

    void ApplyTorque(ClientApplication *app, unsigned int id, float magnitude, std::string direction);
    void ApplyTorque(ClientApplication *app, unsigned int id, float magnitude, glm::vec3 direction);

    void SetVelocity(ClientApplication *app, unsigned int id, std::string direction);
    void SetVelocity(ClientApplication *app, unsigned int id, glm::vec3 direction);

    void Stop(ClientApplication *app, unsigned int id);
}
