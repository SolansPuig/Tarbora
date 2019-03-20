// Fitxer molt temporal, ha de marxar

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_CXX98
#include "glm/glm.hpp"

// Note: This class is from learnopengl.com, with some changes.

class Shader {
    // Reads and compiles two files into a Shader Program, and allows to use and modify it
public:
    unsigned int ID;

    Shader();

    bool LoadFromFile(const char* vertexPath, const char* fragmentPath); // Read and compile the files from those paths into a Shader Program
    void Use(); // Tells the GPU to use that shader program. Should be called at the beggining of every draw call

    // Set the uniform "name" to "value"
    void SetBool(const std::string &name, bool value);
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);
    void SetFloatVec3(const std::string &name, glm::vec3 value);
    void SetFloatVec3(const std::string &name, float x, float y, float z);
    void SetFloatVec4(const std::string &name, glm::vec4 value);
    void SetFloatVec4(const std::string &name, float x, float y, float z, float w);
    void SetMat4(const std::string &name, glm::mat4 value);

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
