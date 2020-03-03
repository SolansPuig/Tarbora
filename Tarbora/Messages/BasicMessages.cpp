#include "BasicMessages.hpp"

namespace Tarbora {
    namespace Message
    {
        Actor::Actor(const MessageBody &m) : Message<::MessageContent::Actor>(m) {}

        Actor::Actor(const ActorId &id)
        {
            message_.set_id(id);
        }

        const ActorId& Actor::getId() { return message_.id(); }

        Node::Node(const MessageBody &m) : Message<::MessageContent::Node>(m) {}

        Node::Node(const ActorId &id, const std::string &name)
        {
            message_.set_id(id);
            message_.set_name(name);
        }

        const ActorId& Node::getId() { return message_.id(); }
        const std::string& Node::getName() { return message_.name(); }

        CreateActor::CreateActor(const MessageBody &m) : Message<::MessageContent::CreateActor>(m) {}

        CreateActor::CreateActor(const ActorId &id, const std::string &entity, const std::string &variant)
        {
            message_.set_id(id);
            message_.set_entity(entity);
            message_.set_variant(variant);
        }

        CreateActor::CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::vec3 &position)
        {
            message_.set_id(id);
            message_.set_entity(entity);
            message_.set_variant(variant);
            (*message_.mutable_position()) = toMessage(position);
        }

        CreateActor::CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::quat &rotation)
        {
            message_.set_id(id);
            message_.set_entity(entity);
            message_.set_variant(variant);
            (*message_.mutable_rotation()) = toMessage(rotation);
        }

        CreateActor::CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::vec3 &position, const glm::quat &rotation)
        {
            message_.set_id(id);
            message_.set_entity(entity);
            message_.set_variant(variant);
            (*message_.mutable_position()) = toMessage(position);
            (*message_.mutable_rotation()) = toMessage(rotation);
        }

        const ActorId& CreateActor::getId() { return message_.id(); }
        const std::string& CreateActor::getEntity() { return message_.entity(); }
        const std::string& CreateActor::getVariant() { return message_.variant(); }
        glm::vec3 CreateActor::getPosition() { return toMath(message_.position()); }
        glm::quat CreateActor::getRotation() { return toMath(message_.rotation()); }
        bool CreateActor::hasPosition() { return message_.has_position(); }
        bool CreateActor::hasRotation() { return message_.has_rotation(); }

//         class CreateActorModel : public Message<::MessageContent::CreateActorModel>
//         {
//         public:
//             CreateActorModel(const MessageBody &m) : Message<::MessageContent::CreateActorModel>(m) {}
//
//             CreateActorModel(const ActorId &id, const std::string &model, const std::string &material, int render_pass)
//             {
//                 message_.set_id(id);
//                 message_.set_model(model);
//                 message_.set_material(material);
//                 message_.set_render_pass(render_pass);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             const std::string& getModel() { return message_.model(); }
//             const std::string& getMaterial() { return message_.material(); }
//             int getRenderPass() { return message_.render_pass(); }
//         };
//
//         class MoveActor : public Message<::MessageContent::MoveActor>
//         {
//         public:
//             MoveActor(const MessageBody &m) : Message<::MessageContent::MoveActor>(m) {}
//
//             MoveActor(const ActorId &id, const glm::vec3 &position)
//             {
//                 message_.set_id(id);
//                 (*message_.mutable_position()) = toMessage(position);
//             }
//
//             MoveActor(const ActorId &id, const glm::quat &rotation)
//             {
//                 message_.set_id(id);
//                 (*message_.mutable_rotation()) = toMessage(rotation);
//             }
//
//             MoveActor(const ActorId &id, const glm::vec3 &position, const glm::quat &rotation)
//             {
//                 message_.set_id(id);
//                 (*message_.mutable_position()) = toMessage(position);
//                 (*message_.mutable_rotation()) = toMessage(rotation);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             glm::vec3 getPosition() { return toMath(message_.position()); }
//             glm::quat getRotation() { return toMath(message_.rotation()); }
//             bool hasPosition() { return message_.has_position(); }
//             bool hasRotation() { return message_.has_rotation(); }
//         };
//
//         class MoveNode : public Message<::MessageContent::MoveNode>
//         {
//         public:
//             MoveNode(const MessageBody &m) : Message<::MessageContent::MoveNode>(m) {}
//
//             MoveNode(const ActorId &id, const std::string &name, const glm::vec3 &position)
//             {
//                 message_.set_id(id);
//                 message_.set_name(name);
//                 (*message_.mutable_position()) = toMessage(position);
//             }
//
//             MoveNode(const ActorId &id, const std::string &name, const glm::quat &rotation)
//             {
//                 message_.set_id(id);
//                 message_.set_name(name);
//                 (*message_.mutable_rotation()) = toMessage(rotation);
//             }
//
//             MoveNode(const ActorId &id, const std::string &name, const glm::vec3 &position, const glm::quat &rotation)
//             {
//                 message_.set_id(id);
//                 message_.set_name(name);
//                 (*message_.mutable_position()) = toMessage(position);
//                 (*message_.mutable_rotation()) = toMessage(rotation);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             const std::string& getName() { return message_.name(); }
//             glm::vec3 getPosition() { return toMath(message_.position()); }
//             glm::quat getRotation() { return toMath(message_.rotation()); }
//             bool hasPosition() { return message_.has_position(); }
//             bool hasRotation() { return message_.has_rotation(); }
//         };
//
//         class SetAnimation : public Message<::MessageContent::SetAnimation>
//         {
//         public:
//             SetAnimation(const MessageBody &m) : Message<::MessageContent::SetAnimation>(m) {}
//
//             SetAnimation(const ActorId &id, const std::string animation)
//             {
//                 message_.set_id(id);
//                 message_.set_animation(animation);
//             }
//
//             SetAnimation(const ActorId &id, const std::string animation, const std::string file)
//             {
//                 message_.set_id(id);
//                 message_.set_animation(animation);
//                 message_.set_file(file);
//             }
//
//             SetAnimation(const ActorId &id, const std::string animation, float speed)
//             {
//                 message_.set_id(id);
//                 message_.set_animation(animation);
//                 message_.set_speed(speed);
//             }
//
//             SetAnimation(const ActorId &id, const std::string animation, const std::string file, float speed)
//             {
//                 message_.set_id(id);
//                 message_.set_animation(animation);
//                 message_.set_file(file);
//                 message_.set_speed(speed);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             const std::string& getAnimation() { return message_.animation(); }
//             const std::string& getFile() { return message_.file(); }
//             float getSpeed() { return message_.speed(); }
//         };
//
//         class ApplyPhysics : public Message<::MessageContent::ApplyPhysics>
//         {
//         public:
//             ApplyPhysics(const MessageBody &m) : Message<::MessageContent::ApplyPhysics>(m) {}
//
//             ApplyPhysics(const ActorId &id, const glm::vec3 &direction)
//             {
//                 message_.set_id(id);
//                 (*message_.mutable_direction()) = toMessage(direction);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             glm::vec3 getDirection() { return toMath(message_.direction()); }
//             bool hasDirection() { return message_.has_direction(); }
//         };
//
//         class LookAt : public Message<::MessageContent::LookAt>
//         {
//         public:
//             LookAt(const MessageBody &m) : Message<::MessageContent::LookAt>(m) {}
//
//             LookAt(const ActorId &id, const glm::vec3 &direction)
//             {
//                 message_.set_id(id);
//                 (*message_.mutable_direction()) = toMessage(direction);
//             }
//
//             LookAt(const ActorId &id, const ActorId &target)
//             {
//                 message_.set_id(id);
//                 message_.set_target(target);
//             }
//
//             LookAt(const ActorId &id, const ActorId &target, float distance)
//             {
//                 message_.set_id(id);
//                 message_.set_target(target);
//                 message_.set_distance(distance);
//             }
//
//             const ActorId& getId() { return message_.id(); }
//             const ActorId& getTarget() { return message_.target(); }
//             float getDistance() { return message_.distance(); }
//             glm::vec3 getDirection() { return toMath(message_.direction()); }
//             bool hasDirection() { return message_.has_direction(); }
//         };
    }
}
