#include "ModelEditor.hpp"
#include "HumanView.hpp"
#include "GameLayer.hpp"
#include "ActorModel.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    ModelEditor::ModelEditor(HumanView *view, bool start_active)
        : Layer(view, start_active)
    {
        actor_id_ = "";
        captured_ = false;
        scene_ = view->getGameLayer()->getScene();

        subscribe("look_at", [&](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::LookAt m(body);
            if (m.getId() == "player" && !captured_)
            {
                actor_id_ = m.getTarget();
                if (actor_id_ != "")
                {
                    model_ = std::static_pointer_cast<ActorModel>(scene_->getActor(actor_id_));
                    model_->animate("");
                }
            }
        });
    }

    void ModelEditor::getInput()
    {
        if (getInputManager()->getKeyDown(KEY_F4)) {
            active_ = !active_;
        }
    }

    void ModelEditor::nodeInspector(std::shared_ptr<SceneNode> node)
    {
        for (auto child : *node)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (selected_node_ && selected_node_->getName() == child.first.c_str())
                flags |= ImGuiTreeNodeFlags_Selected;
            bool open = ImGui::TreeNodeEx(child.first.c_str(), flags);
            if (ImGui::IsItemClicked())
            {
                selected_node_ = child.second;
            }
            if (open)
            {
                nodeInspector(child.second);
                ImGui::TreePop();
            }
        }
    }

    void ModelEditor::draw()
    {
        if (active_)
        {
            if (actor_id_ == "")
                captured_ = false;

            ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
            ImGui::Begin("Model Editor", &active_, ImGuiWindowFlags_None);

            ImGui::BeginGroup();
            ImGui::Text("Target: %s", actor_id_.c_str());
            ImGui::SameLine(400.f);
            if (actor_id_ != "") ImGui::Checkbox("Captured", &captured_);
            ImGui::EndGroup();

            ImGui::Separator();

            const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("Node Inspector", ImVec2(0, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (actor_id_ != "" && model_)
            {
                if (ImGui::CollapsingHeader("Options"))
                {
                    ImGui::Spacing();

                    if (ImGui::Button("Reload"))
                    {
                        // scene_->removeActor(actor_id_);
                    }

                    ImGui::Spacing();

                    const float step = 0.1f;
                    float scale = model_->getGlobalScale();
                    if (ImGui::InputScalar("Global Scale", ImGuiDataType_Float, &scale, &step))
                        model_->setGlobalScale(scale);
                }

                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Nodes"))
                {
                    nodeInspector(model_);
                }
            }

            ImGui::EndChild();
            ImGui::SetItemDefaultFocus();
            ImGui::End();

            ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
            ImGui::Begin("Node Editor", &active_, ImGuiWindowFlags_None);

            if (selected_node_)
            {
                ImGui::Text("Node: %s", selected_node_->getName().c_str());
                ImGui::Separator();

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
                if (ImGui::CollapsingHeader("Transform", flags))
                {
                    ImGui::Spacing();
                    glm::vec3 p = selected_node_->getPosition()*100.f;
                    float position[3] = {p.x, p.y, p.z};
                    if (ImGui::DragFloat3("Position", position, 1.f, 0.f, 0.f, "%.1f cm"))
                        selected_node_->setPosition(glm::make_vec3(position)/100.f);

                    // TODO: Fix gimbal lock
                    ImGui::Spacing();
                    glm::vec3 r = selected_node_->getRotation();
                    float rotation[3] = {r.x, r.y, r.z};
                    if (ImGui::DragFloat3("Rotation", rotation, 1.f, -180.f, 180.f, "%.1f°"))
                        selected_node_->setRotation(glm::make_vec3(rotation));

                    ImGui::Spacing();
                    glm::vec3 s = selected_node_->getScale()*100.f;
                    float scale[3] = {s.x, s.y, s.z};
                    if (ImGui::DragFloat3("Scale", scale, 1.f, 1.f, 1000.f, "%.1f cm"))
                        selected_node_->setScale(glm::make_vec3(scale)/100.f);
                }

                if (selected_node_->getNodeType() == "MESH")
                {
                    auto node = std::static_pointer_cast<MeshNode>(selected_node_);

                    ImGui::Spacing();

                    if (ImGui::CollapsingHeader("Colors"))
                    {
                        ImGui::Spacing();
                        glm::vec3 c1 = node->getColorPrimary();
                        c1 /= 255.f;
                        float primary[3] = {c1.x, c1.y, c1.z};
                        if (ImGui::ColorEdit3("Color Primary", &primary[0]))
                            node->setColorPrimary(glm::make_vec3(primary)*255.f);

                        ImGui::Spacing();
                        glm::vec3 c2 = node->getColorSecondary();
                        c2 /= 255.f;
                        float secondary[3] = {c2.x, c2.y, c2.z};
                        if (ImGui::ColorEdit3("Color Secondary", &secondary[0]))
                            node->setColorSecondary(glm::make_vec3(secondary)*255.f);

                        ImGui::Spacing();
                        glm::vec3 c3 = node->getColorDetail();
                        c3 /= 255.f;
                        float detail[3] = {c3.x, c3.y, c3.z};
                        if (ImGui::ColorEdit3("Color Detail", &detail[0]))
                            node->setColorDetail(glm::make_vec3(detail)*255.f);

                        ImGui::Spacing();
                        glm::vec3 c4 = node->getColorDetail2();
                        c4 /= 255.f;
                        float detail2[3] = {c4.x, c4.y, c4.z};
                        if (ImGui::ColorEdit3("Color Detail 2", &detail2[0]))
                            node->setColorDetail2(glm::make_vec3(detail2)*255.f);
                    }

                    ImGui::Spacing();

                    if (ImGui::CollapsingHeader("Texture"))
                    {
                        ImGui::Spacing();
                        glm::tvec2<int> uv = node->getUvMap();
                        int uv_map[2] = {uv.x, uv.y};
                        if (ImGui::DragInt2("UV Map", uv_map))
                            node->setUvMap(glm::make_vec2(uv_map));
                    }
                }
            }

            ImGui::End();
        }

    }
}
