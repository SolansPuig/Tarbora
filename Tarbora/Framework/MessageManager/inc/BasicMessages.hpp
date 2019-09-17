#pragma once

#include "MathMessages.hpp"
#include "../proto/basic.pb.h"

using namespace tbBasics;

namespace Tarbora {
    class Module;

    void CreateActor(Module *app, std::string entity, std::string variant, std::string position, std::string rotation="0,0,0");
    void CreateActor(Module *app, std::string entity, std::string variant, glm::vec3 position, glm::vec3 rotation=glm::vec3(0.f,0.f,0.f));

    void CreateActorModel(Module *app, unsigned int id, std::string entity, std::string variant);

    void DeleteActor(Module *app, unsigned int id);

    void SetCamera(Module *app, unsigned int actorId, std::string name);

    void MoveActor(Module *app, unsigned int id, std::string position, std::string rotation);
    void MoveActor(Module *app, unsigned int id, glm::vec3 position, glm::mat3 rotation);

    void AnimateActor(Module *app, unsigned int id, std::string animation);

    void ApplyForce(Module *app, unsigned int id, float magnitude, std::string direction);
    void ApplyForce(Module *app, unsigned int id, float magnitude, glm::vec3 direction);

    void ApplyTorque(Module *app, unsigned int id, float magnitude, std::string direction);
    void ApplyTorque(Module *app, unsigned int id, float magnitude, glm::vec3 direction);

    void SetVelocity(Module *app, unsigned int id, std::string direction);
    void SetVelocity(Module *app, unsigned int id, glm::vec3 direction);

    void Stop(Module *app, unsigned int id);
}
