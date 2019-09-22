#include "../inc/GraphicsEngine.hpp"
#include "../imgui/examples/imgui_impl_glfw.h"
#include "../imgui/examples/imgui_impl_opengl3.h"
#include "../../GraphicViews/inc/GraphicView.hpp"

namespace Tarbora {
    Gui::Gui(GraphicView *view) :
        m_View(view)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../Resources/fonts/Roboto-Medium.ttf", 16.0f);
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        // ImGuiStyle& style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL(m_View->GraphicsEngine()->GetWindow()->GetRawWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    Gui::~Gui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::BeforeDraw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::AfterDraw()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
