#include "GraphicsEngine.hpp"
#include "External/imgui/examples/imgui_impl_glfw.h"
#include "External/imgui/examples/imgui_impl_opengl3.h"

namespace Tarbora {
    Gui::Gui(GraphicsEngine *graphicsEngine) :
        graphics_engine_(graphicsEngine)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Resources/fonts/Roboto-Medium.ttf", 16.0f);
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        // ImGuiStyle& style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL(graphicsEngine->getWindow()->getRawWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    Gui::~Gui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::beforeDraw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::afterDraw()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
