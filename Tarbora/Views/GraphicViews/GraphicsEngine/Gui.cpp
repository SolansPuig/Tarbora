/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

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
    static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;

    io.Fonts->AddFontFromFileTTF(
      "../Resources/fonts/forkawesome-webfont.ttf", 16.0f, &icons_config, icons_ranges
    );

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
    ImGuizmo::BeginFrame();
  }

  void Gui::afterDraw()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
}
