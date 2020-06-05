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

#ifndef __METRICS_GUI_H_
#define __METRICS_GUI_H_

#include "Layer.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
  class MetricsGui : public Layer
  {
  public:
    MetricsGui(GraphicView *view, bool start_active)
      : Layer(view, start_active)
    {
      subscribe("look_at", [&](const MessageSubject &, const MessageBody &body)
      {
        Message::LookAt m(body);
        if (m->id() == "player")
          target_ = m->target();
      });
    }

    void getInput() override
    {
      if (getInputManager()->getKeyDown(KEY_F3)) {
        active_ = !active_;
      }
    }

    void update(float delta_time) override
    {
      delta_time_ = delta_time;
    }

    void draw() override
    {
      const float DISTANCE = 10.f;

      ImVec2 window_pos = ImVec2(
        ImGui::GetWindowPos().x + DISTANCE,
        ImGui::GetWindowPos().y + DISTANCE
      );
      ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
      ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
      ImGui::SetNextWindowBgAlpha(0.3f);
      ImGui::Begin(
        "Metrics", &active_,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav
      );
      ImGui::Text("FPS: %.0f", 1/delta_time_);
      ImGui::Text("Looking at: %s", target_.c_str());
      ImGui::End();
    }

    void SetTime(float delta_time)
    {
      delta_time_ = delta_time * 1000;
    }

  private:
    float delta_time_{0.f};
    std::string target_{""};
  };
}

#endif // __METRICS_GUI_H_
