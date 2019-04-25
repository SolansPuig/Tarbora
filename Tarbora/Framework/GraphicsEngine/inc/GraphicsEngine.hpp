#pragma once
#include "Gui.hpp"
#include "Input.hpp"
#include "KeyCodes.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Tarbora {
    namespace GraphicsEngine {
        void Init();
        void Close();

        void BeforeDraw();
        void DrawMesh(std::shared_ptr<Mesh> mesh);
        void AfterDraw();

        void BeforeDrawSky();
        void AfterDrawSky();

        WindowPtr GetWindow();

        unsigned int CompileShader(std::string type, const char *code);
        unsigned int LinkProgram(unsigned int *ids);
        void DeleteProgram(unsigned int id);

        unsigned int LoadTexture(unsigned char *data, int width, int height, int nrComponents);
        void BindTexture(unsigned int id);
        void DeleteTexture(unsigned int id);

        unsigned int LoadMesh(std::vector<float> vertices);
        void DeleteMesh(unsigned int id);

        void UseShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> GetShader();

        int TakeScreenshot(const std::string &filename);
    }
}
