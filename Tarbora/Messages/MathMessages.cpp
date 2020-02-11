#include "MathMessages.hpp"

namespace Tarbora {
    MathMessage::Vec2 Vec2(glm::vec2 data)
    {
        MathMessage::Vec2 vec;
        vec.set_x(data.x);
        vec.set_y(data.y);

        return vec;
    }

    MathMessage::Vec2 Vec2(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ',');
            words.push_back(word);
        }

        MathMessage::Vec2 vec;
        vec.set_x(std::stof(words[0]));
        vec.set_y(std::stof(words[1]));
        return vec;
    }

    glm::vec2 Vec2toGLM(MathMessage::Vec2 data)
    {
        return glm::vec2(data.x(), data.y());
    }

    MathMessage::Vec3 Vec3(glm::vec3 data)
    {
        MathMessage::Vec3 vec;
        vec.set_x(data.x);
        vec.set_y(data.y);
        vec.set_z(data.z);

        return vec;
    }

    MathMessage::Vec3 Vec3(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ',');
            words.push_back(word);
        }

        MathMessage::Vec3 vec;
        vec.set_x(std::stof(words[0]));
        vec.set_y(std::stof(words[1]));
        vec.set_z(std::stof(words[2]));

        return vec;
    }

    glm::vec3 Vec3toGLM(MathMessage::Vec3 data)
    {
        return glm::vec3(data.x(), data.y(), data.z());
    }

    MathMessage::Vec4 Vec4(glm::vec4 data)
    {
        MathMessage::Vec4 vec;
        vec.set_x(data.x);
        vec.set_y(data.y);
        vec.set_z(data.z);
        vec.set_w(data.w);

        return vec;
    }

    MathMessage::Vec4 Vec4(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ',');
            words.push_back(word);
        }

        MathMessage::Vec4 vec;
        vec.set_x(std::stof(words[0]));
        vec.set_y(std::stof(words[1]));
        vec.set_z(std::stof(words[2]));
        vec.set_w(std::stof(words[3]));

        return vec;
    }

    glm::vec4 Vec4toGLM(MathMessage::Vec4 data)
    {
        return glm::vec4(data.x(), data.y(), data.z(), data.w());
    }


    MathMessage::Mat3 Mat3(glm::mat3 data)
    {
        MathMessage::Mat3 mat;
        MathMessage::Vec3 *vec = mat.mutable_x();
        (*vec) = Vec3(data[0]);
        vec = mat.mutable_y();
        (*vec) = Vec3(data[1]);
        vec = mat.mutable_z();
        (*vec) = Vec3(data[2]);

        return mat;
    }

    MathMessage::Mat3 Mat3(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ';');
            words.push_back(word);
        }

        MathMessage::Mat3 mat;
        MathMessage::Vec3 *vec = mat.mutable_x();
        (*vec) = Vec3(words[0]);
        vec = mat.mutable_y();
        (*vec) = Vec3(words[1]);
        vec = mat.mutable_z();
        (*vec) = Vec3(words[2]);

        return mat;
    }

    glm::mat3 Mat3toGLM(MathMessage::Mat3 data)
    {
        return glm::mat3(
            data.x().x(), data.x().y(), data.x().z(),
            data.y().x(), data.y().y(), data.y().z(),
            data.z().x(), data.z().y(), data.z().z()
        );
    }

    MathMessage::Mat4 Mat4(glm::mat4 data)
    {
        MathMessage::Mat4 mat;
        MathMessage::Vec4 *vec = mat.mutable_x();
        (*vec) = Vec4(data[0]);
        vec = mat.mutable_y();
        (*vec) = Vec4(data[1]);
        vec = mat.mutable_z();
        (*vec) = Vec4(data[2]);
        vec = mat.mutable_w();
        (*vec) = Vec4(data[3]);

        return mat;
    }

    MathMessage::Mat4 Mat4(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ';');
            words.push_back(word);
        }

        MathMessage::Mat4 mat;
        MathMessage::Vec4 *vec = mat.mutable_x();
        (*vec) = Vec4(words[0]);
        vec = mat.mutable_y();
        (*vec) = Vec4(words[1]);
        vec = mat.mutable_z();
        (*vec) = Vec4(words[2]);
        vec = mat.mutable_w();
        (*vec) = Vec4(words[3]);

        return mat;
    }

    glm::mat4 Mat4toGLM(MathMessage::Mat4 data)
    {
        return glm::mat4(
            data.x().x(), data.x().y(), data.x().z(), data.x().w(),
            data.y().x(), data.y().y(), data.y().z(), data.y().w(),
            data.z().x(), data.z().y(), data.z().z(), data.z().w(),
            data.w().x(), data.w().y(), data.w().z(), data.w().w()
        );
    }
}
