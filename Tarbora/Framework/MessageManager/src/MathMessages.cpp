#include "../inc/MathMessages.hpp"

using tbMath::Vec3Message;
using tbMath::Vec4Message;
using tbMath::Mat3Message;
using tbMath::Mat4Message;

namespace Tarbora {

    Vec3Message Vec3(glm::vec3 data)
    {
        Vec3Message vec;
        vec.set_x(data.x);
        vec.set_y(data.y);
        vec.set_z(data.z);

        return vec;
    }

    Vec3Message Vec3(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ',');
            words.push_back(word);
        }

        Vec3Message vec;
        vec.set_x(std::stof(words[0]));
        vec.set_y(std::stof(words[1]));
        vec.set_z(std::stof(words[2]));

        return vec;
    }

    glm::vec3 Vec3toGLM(Vec3Message data)
    {
        return glm::vec3(data.x(), data.y(), data.z());
    }

    Vec4Message Vec4(glm::vec4 data)
    {
        Vec4Message vec;
        vec.set_x(data.x);
        vec.set_y(data.y);
        vec.set_z(data.z);
        vec.set_w(data.w);

        return vec;
    }

    Vec4Message Vec4(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ',');
            words.push_back(word);
        }

        Vec4Message vec;
        vec.set_x(std::stof(words[0]));
        vec.set_y(std::stof(words[1]));
        vec.set_z(std::stof(words[2]));
        vec.set_w(std::stof(words[3]));

        return vec;
    }

    glm::vec4 Vec4toGLM(Vec4Message data)
    {
        return glm::vec4(data.x(), data.y(), data.z(), data.w());
    }


    Mat3Message Mat3(glm::mat3 data)
    {
        Mat3Message mat;
        Vec3Message *vec = mat.mutable_x();
        (*vec) = Vec3(data[0]);
        vec = mat.mutable_y();
        (*vec) = Vec3(data[1]);
        vec = mat.mutable_z();
        (*vec) = Vec3(data[2]);

        return mat;
    }

    Mat3Message Mat3(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ';');
            words.push_back(word);
        }

        Mat3Message mat;
        Vec3Message *vec = mat.mutable_x();
        (*vec) = Vec3(words[0]);
        vec = mat.mutable_y();
        (*vec) = Vec3(words[1]);
        vec = mat.mutable_z();
        (*vec) = Vec3(words[2]);

        return mat;
    }

    glm::mat3 Mat3toGLM(Mat3Message data)
    {
        return glm::mat3(
            data.x().x(), data.x().y(), data.x().z(),
            data.y().x(), data.y().y(), data.y().z(),
            data.z().x(), data.z().y(), data.z().z()
        );
    }

    Mat4Message Mat4(glm::mat4 data)
    {
        Mat4Message mat;
        Vec4Message *vec = mat.mutable_x();
        (*vec) = Vec4(data[0]);
        vec = mat.mutable_y();
        (*vec) = Vec4(data[1]);
        vec = mat.mutable_z();
        (*vec) = Vec4(data[2]);
        vec = mat.mutable_w();
        (*vec) = Vec4(data[3]);

        return mat;
    }

    Mat4Message Mat4(std::string data)
    {
        std::stringstream buf(data);
        std::vector<std::string> words;
        while (buf.good())
        {
            std::string word;
            getline(buf, word, ';');
            words.push_back(word);
        }

        Mat4Message mat;
        Vec4Message *vec = mat.mutable_x();
        (*vec) = Vec4(words[0]);
        vec = mat.mutable_y();
        (*vec) = Vec4(words[1]);
        vec = mat.mutable_z();
        (*vec) = Vec4(words[2]);
        vec = mat.mutable_w();
        (*vec) = Vec4(words[3]);

        return mat;
    }

    glm::mat4 Mat4toGLM(Mat4Message data)
    {
        return glm::mat4(
            data.x().x(), data.x().y(), data.x().z(), data.x().w(),
            data.y().x(), data.y().y(), data.y().z(), data.y().w(),
            data.z().x(), data.z().y(), data.z().z(), data.z().w(),
            data.w().x(), data.w().y(), data.w().z(), data.w().w()
        );
    }
}
