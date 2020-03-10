#include "Editor.hpp"
#include "../Views/GraphicViews/HumanView.hpp"
#include "../Views/GraphicViews/GameLayer.hpp"
#include "../Views/GraphicViews/ActorModel.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    Editor::Editor(HumanView *view, bool start_active)
        : Layer(view, start_active)
    {
        scene_ = view->getGameLayer()->getScene();

        model_editor = std::shared_ptr<ModelEditor>(new ModelEditor(this, scene_));
        node_editor = std::shared_ptr<NodeEditor>(new NodeEditor(this));

        subscribe("look_at", [&](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::LookAt m(body);
            if (m.getId() == "player")
                model_editor->setTarget(m.getTarget());
        });
    }

    void Editor::getInput()
    {
        if (getInputManager()->getKeyDown(KEY_F4)) {
            active_ = !active_;
        }
    }

    void Editor::draw()
    {
        if (active_)
        {
            model_editor->draw(&active_);
            node_editor->draw(&active_);
        }
    }

    ModelEditor::ModelEditor(Editor *editor, std::shared_ptr<Scene> scene)
    {
        editor_ = editor;
        scene_ = scene;
        captured_ = false;
    }

    void ModelEditor::setTarget(const ActorId &id)
    {
        if (!captured_)
        {
            if (actor_id_ != id)
            {
                actor_id_ = id;
                model_ = std::static_pointer_cast<ActorModel>(scene_->getActor(actor_id_));
                editor_->node_editor->setTarget("", std::shared_ptr<SceneNode>());
            }

        }
    }

    void ModelEditor::draw(bool *active)
    {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
        ImGui::Begin("Model Editor", active, ImGuiWindowFlags_None);

        // Header
        ImGui::BeginGroup();
        ImGui::Text("Target: %s", actor_id_.c_str());
        ImGui::SameLine(400.f);
        if (actor_id_ != "") ImGui::Checkbox("Captured", &captured_);
        ImGui::EndGroup();

        ImGui::Separator();

        // Inspector
        const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("Inspector", ImVec2(0, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (auto model = model_.lock())
        {
            // Options
            if (ImGui::CollapsingHeader("Options"))
            {
                ImGui::Spacing();

                if (ImGui::Button("Reload"))
                {
                    // TODO: Change that
                    glm::vec3 position = model->getPosition();
                    glm::quat rotation = model->getRotation();
                    ResourceManager::flush();
                    model_ = scene_->createActorModel(actor_id_, model->getRenderPass(), model->getModel(), model->getMaterial());
                    model = model_.lock();
                    model->setPosition(position);
                    model->setRotation(rotation);
                }

                ImGui::SameLine();

                if (ImGui::Button("Load from..."))
                {
                    ImGui::OpenPopup("Select File");
                }

                ImGui::SameLine();

                if (ImGui::Button("Save"))
                {
                    ImGui::OpenPopup("Save to File");
                }

                if (ImGui::BeginPopupModal("Select File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static std::string file = "example.lua";
                    ImGui::InputText("File Name", &file);

                    ImGui::Separator();

                    // Submit or cancel the modal
                    if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    {
                        file = "example.lua";
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Ok", ImVec2(120, 0)) && file != "")
                    {
                        // TODO: Change that
                        glm::vec3 position = model->getPosition();
                        glm::quat rotation = model->getRotation();
                        ResourceManager::flush();
                        model_ = scene_->createActorModel(actor_id_, model->getRenderPass(), file, model->getMaterial());
                        model = model_.lock();
                        model->setPosition(position);
                        model->setRotation(rotation);

                        file = "example.lua";
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal("Save to File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::TextUnformatted("Caution! This function could overwrite files that have extended functionality, losing it. Always keep a copy!");
                    static std::string file = "example.lua";
                    ImGui::InputText("File Name", &file);

                    ImGui::Separator();

                    // Submit or cancel the modal
                    if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    {
                        file = "example.lua";
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Ok", ImVec2(120, 0)) && file != "")
                    {
                        ModelSaver saver("../Resources/models/" + file, model);
                        file = "example.lua";
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::Spacing();

                const float step = 0.1f;
                float scale = model->getGlobalScale();
                if (ImGui::InputScalar("Global Scale", ImGuiDataType_Float, &scale, &step))
                    model->setGlobalScale(scale);
            }

            ImGui::Spacing();

            // Nodes
            if (ImGui::CollapsingHeader("Nodes"))
            {
                nodeInspector(model);
            }
        }

        ImGui::EndChild();
        ImGui::SetItemDefaultFocus();
        ImGui::End();
    }

    void ModelEditor::nodeInspector(std::shared_ptr<SceneNode> node)
    {
        unsigned int menu_action = 0;

        for (auto child : *node)
        {
            // Flags. Check if it's selected
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (auto s = selected_node_.lock())
                if (s->getName() == child.first.c_str())
                    flags |= ImGuiTreeNodeFlags_Selected;

            ImGui::PushID(child.first.c_str()); // For Drag N Drop

            // Node tree
            bool open = ImGui::TreeNodeEx(child.first.c_str(), flags);

            // Allow to drag and drop nodes
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                ImGui::SetDragDropPayload("DND_NODE_TREE", &(child.second), sizeof(std::shared_ptr<SceneNode>));
                ImGui::Text("Move %s", child.first.c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_NODE_TREE"))
                {
                    auto dragged_node = *(const std::shared_ptr<SceneNode>*)payload->Data;
                    auto parent = dragged_node->getParent();

                    if (parent != child.second.get())
                    {
                        child.second->addChild(dragged_node);
                        parent->removeChild(dragged_node->getName());
                    }

                    LOG_DEBUG("Dragged %s to %s", dragged_node->getName().c_str(), child.first.c_str());
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::PopID(); // For Drag N Drop

            if (ImGui::IsItemClicked()) // Select node if clicked
            {
                selected_node_ = child.second;
                editor_->node_editor->setTarget(child.first, child.second);
            }
            // Open dialog menu if right-clicked
            if (ImGui::BeginPopupContextItem(child.first.c_str()))
            {
                edited_node_ = child.second;

                if (ImGui::BeginMenu("New node")) // Create a new node
                {
                    if (ImGui::MenuItem("Mesh")) menu_action = 1;
                    if (ImGui::MenuItem("Material")) menu_action = 2;
                    if (ImGui::MenuItem("Camera")) menu_action = 3;
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Delete node")) menu_action = 4;
                if (ImGui::MenuItem("Rename")) menu_action = 5;
                ImGui::EndPopup();
            }
            if (open)
            {
                nodeInspector(child.second);
                ImGui::TreePop(); // TODO: Move that
            }
        }

        if (auto edited = edited_node_.lock())
        {
            // Perform the menu actions
            if (menu_action == 1) ImGui::OpenPopup("New Mesh");
            if (menu_action == 2) ImGui::OpenPopup("New Material");
            if (menu_action == 3) ImGui::OpenPopup("New Camera");
            if (menu_action == 4) node->removeChild(edited->getName());
            if (menu_action == 5) ImGui::OpenPopup("Rename");

            if (ImGui::BeginPopupModal("New Mesh", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                static std::string name = "";
                ImGui::InputText("Name", &name);

                static std::string shape = "cube.mesh";
                ImGui::InputText("Shape", &shape); // TODO: Show a selector with all the available files

                // Pick a render pass
                static unsigned int render_pass = model_.lock()->getRenderPass();
                const char* passes[] = { "Static", "Actor", "No-Culling", "Sky", "Transparent", "Invisible" };
                if (ImGui::Button("Render Pass"))
                    ImGui::OpenPopup("Render Pass Popup");
                ImGui::SameLine();
                ImGui::TextUnformatted(passes[render_pass]);
                if (ImGui::BeginPopup("Render Pass Popup"))
                {
                    ImGui::Text("Render Pass");
                    ImGui::Separator();
                    for (int i = 0; i < IM_ARRAYSIZE(passes); i++)
                        if (ImGui::Selectable(passes[i]))
                            render_pass = i;
                    ImGui::EndPopup();
                }

                ImGui::Separator();

                // Submit or cancel the modal
                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();
                ImGui::SameLine();
                if (ImGui::Button("Ok", ImVec2(120, 0)) && name != "")
                {
                    auto n = std::shared_ptr<MeshNode>(new MeshNode(
                        edited->getActorId(),
                        name,
                        (RenderPass)render_pass,
                        shape
                    ));
                    edited->addChild(n);

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopupModal("New Material", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                static std::string name = "";
                ImGui::InputText("Name", &name);

                static std::string material = "white.mat.lua";
                ImGui::InputText("Material", &material); // TODO: Show a selector with all the available files

                ImGui::Separator();

                // Submit or cancel the modal
                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();
                ImGui::SameLine();
                if (ImGui::Button("Ok", ImVec2(120, 0)) && name != "")
                {
                    auto n = std::shared_ptr<MaterialNode>(new MaterialNode(
                        edited->getActorId(),
                        name,
                        material
                    ));
                    edited->addChild(n);

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }

    NodeEditor::NodeEditor(Editor *editor)
    {
        editor_ = editor;
    }

    void NodeEditor::setTarget(const std::string &name, std::shared_ptr<SceneNode> node)
    {
        node_name_ = name;
        node_ = node;

        // Configure auto texture size
        auto_texture_size_ = false;
        if (auto node = node_.lock())
        {
            if (node->getNodeType() == "ANIMATED")
            {
                auto mesh = std::static_pointer_cast<MeshNode>(node);
                auto_texture_size_ = mesh->getTextureSize() == mesh->getScale();
            }
        }
    }

    void NodeEditor::draw(bool *active)
    {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
        ImGui::Begin("Node Editor", active, ImGuiWindowFlags_None);

        // Header
        ImGui::Text("Node: %s", node_name_.c_str());

        ImGui::Separator();

        if (auto node = node_.lock())
        {
            // Transform of this node
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Spacing();
                glm::vec3 p = node->getPosition()*100.f;
                float position[3] = {p.x, p.y, p.z};
                if (ImGui::DragFloat3("Position", position, 1.f, 0.f, 0.f, "%.2f cm"))
                    node->setPosition(glm::make_vec3(position)/100.f);

                // TODO: Fix gimbal lock
                ImGui::Spacing();
                glm::vec3 r = node->getRotation();
                float rotation[3] = {r.x, r.y, r.z};
                if (ImGui::DragFloat3("Rotation", rotation, 1.f, -180.f, 180.f, "%.2f°"))
                    node->setRotation(glm::make_vec3(rotation));

                ImGui::Spacing();
                glm::vec3 s = node->getScale()*100.f;
                float scale[3] = {s.x, s.y, s.z};
                if (ImGui::DragFloat3("Scale", scale, 1.f, 1.f, 1000.f, "%.2f cm"))
                {
                    glm::vec3 size = glm::make_vec3(scale)/100.f;
                    node->setScale(size);
                    if (node->getNodeType() == "ANIMDATED")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        mesh->setMeshSize(size);
                        if (auto_texture_size_) mesh->setTextureSize(size);
                    }
                }

                ImGui::Spacing();
                float g_scale = node->getGlobalScale();
                if (ImGui::DragFloat("Global Scale", &g_scale, 0.1f, 0.1f, 1000.f, "%.2f"))
                    node->setGlobalScale(g_scale);

                ImGui::Spacing();
                glm::vec3 o = node->getOrigin();
                float origin[3] = {o.x, o.y, o.z};
                if (ImGui::DragFloat3("Origin", origin, 0.01f, -0.5f, 0.5f, "%.2f"))
                    node->setOrigin(glm::make_vec3(origin));
            }

            if (node->getNodeType() == "ANIMATED")
            {
                auto mesh = std::static_pointer_cast<MeshNode>(node);
                ImGui::Spacing();

                // Colors of the node
                if (ImGui::CollapsingHeader("Colors"))
                {
                    ImGui::Spacing();
                    glm::vec3 c1 = mesh->getColorPrimary();
                    c1 /= 255.f;
                    float primary[3] = {c1.x, c1.y, c1.z};
                    if (ImGui::ColorEdit3("Color Primary", &primary[0]))
                        mesh->setColorPrimary(glm::make_vec3(primary)*255.f);

                    ImGui::Spacing();
                    glm::vec3 c2 = mesh->getColorSecondary();
                    c2 /= 255.f;
                    float secondary[3] = {c2.x, c2.y, c2.z};
                    if (ImGui::ColorEdit3("Color Secondary", &secondary[0]))
                        mesh->setColorSecondary(glm::make_vec3(secondary)*255.f);

                    ImGui::Spacing();
                    glm::vec3 c3 = mesh->getColorDetail();
                    c3 /= 255.f;
                    float detail[3] = {c3.x, c3.y, c3.z};
                    if (ImGui::ColorEdit3("Color Detail", &detail[0]))
                        mesh->setColorDetail(glm::make_vec3(detail)*255.f);

                    ImGui::Spacing();
                    glm::vec3 c4 = mesh->getColorDetail2();
                    c4 /= 255.f;
                    float detail2[3] = {c4.x, c4.y, c4.z};
                    if (ImGui::ColorEdit3("Color Detail 2", &detail2[0]))
                        mesh->setColorDetail2(glm::make_vec3(detail2)*255.f);
                }

                ImGui::Spacing();

                // Texture configuration
                if (ImGui::CollapsingHeader("Texture"))
                {
                    ImGui::Spacing();
                    glm::tvec2<int> uv = mesh->getUvMap();
                    int uv_map[2] = {uv.x, uv.y};
                    if (ImGui::DragInt2("UV Map", uv_map))
                        mesh->setUvMap(glm::make_vec2(uv_map));

                    ImGui::Spacing();
                    if (ImGui::Checkbox("Auto Texture Size", &auto_texture_size_))
                    {
                        if (auto_texture_size_) mesh->setTextureSize(mesh->getScale());
                    }
                    if (!auto_texture_size_)
                    {
                        glm::vec3 ts = mesh->getTextureSize()*100.f;
                        float texture_size[3] = {ts.x, ts.y, ts.z};
                        if (ImGui::DragFloat3("Texture Size", texture_size, 1.f, 1.f, 1000.f, "%.2f cm"))
                            mesh->setTextureSize(glm::make_vec3(texture_size)/100.f);
                    }
                }
            }
        }

        ImGui::End();
    }

    ModelSaver::ModelSaver(const std::string &file, std::shared_ptr<ActorModel> model)
    {
        file_.open(file.c_str());
        indentation_ = 0;

        if (file_.is_open())
        {
            file_ << "scale = " << model->getGlobalScale() << std::endl;
            file_ << "nodes = {" << std::endl;
            indentation_ += 4;
            for (auto n : *model)
                saveNode(n.second);
            file_ << "}" << std::endl;
        }
    }

    void ModelSaver::saveNode(std::shared_ptr<SceneNode> node)
    {
        file_ << std::string(indentation_, ' ') << "{" << std::endl;
        indentation_ += 4;
        file_ << std::string(indentation_, ' ') << "name = \"" << node->getName() << "\"," << std::endl;
        file_ << std::string(indentation_, ' ') << "scale = " << std::fixed << std::setprecision(2) << node->getGlobalScale() << "," << std::endl;
        if (node->getOrigin() != glm::vec3(0.f)) saveVec3("origin", node->getOrigin());
        if (node->getPosition() != glm::vec3(0.f)) saveVec3("position", node->getPosition() * 100.f);
        if (node->getRotation() != glm::vec3(0.f)) saveVec3("rotation", node->getRotation());

        if (node->getNodeType() == "ANIMATED")
        {
            auto mesh = std::static_pointer_cast<MeshNode>(node);
            if (mesh->getScale() != glm::vec3(0.f)) saveVec3("size", mesh->getScale() * 100.f);
            file_ << std::string(indentation_, ' ') << "shape = \"" << mesh->getShape() << "\"," << std::endl;
            saveVec2("uv_map", mesh->getUvMap());
            if (mesh->getTextureSize() != mesh->getScale()) saveVec3("texture_size", mesh->getTextureSize() * 100.f);
            if (mesh->getColorPrimary() != glm::tvec3<unsigned char>(255)) saveVec3("color_primary", mesh->getColorPrimary());
            if (mesh->getColorSecondary() != glm::tvec3<unsigned char>(255)) saveVec3("color_secondary", mesh->getColorSecondary());
            if (mesh->getColorDetail() != glm::tvec3<unsigned char>(255)) saveVec3("color_detail", mesh->getColorDetail());
            if (mesh->getColorDetail2() != glm::tvec3<unsigned char>(255)) saveVec3("color_detail2", mesh->getColorDetail2());
        }
        else if (node->getNodeType() == "CAMERA")
        {
            file_ << std::string(indentation_, ' ') << "type = \"camera\"" << std::endl;
        }
        else if (node->getNodeType() == "MATERIAL")
        {
            file_ << std::string(indentation_, ' ') << "type = \"material\"" << std::endl;
        }

        if (node->size() > 0)
        {
            file_ << std::string(indentation_, ' ') << "nodes = {" << std::endl;
            indentation_ += 4;
            for (auto n : *node)
                saveNode(n.second);
            indentation_ -= 4;
            file_ << std::string(indentation_, ' ') << "}" << std::endl;
        }
        indentation_ -= 4;
        file_ << std::string(indentation_, ' ') << "}," << std::endl;
    }

    void ModelSaver::saveVec3(const std::string &name, const glm::vec3 &vector)
    {
        file_ << std::string(indentation_, ' ') << name << " = "
            << "{" << std::fixed << std::setprecision(2) << vector.x << ", "
            << std::fixed << std::setprecision(2) << vector.y << ", "
            << std::fixed << std::setprecision(2) << vector.z << "}," << std::endl;
    }

    void ModelSaver::saveVec2(const std::string &name, const glm::vec2 &vector)
    {
        file_ << std::string(indentation_, ' ') << name << " = "
            << "{" << std::fixed << std::setprecision(2) << vector.x << ", "
            << std::fixed << std::setprecision(2) << "}," << std::endl;
    }

    void ModelSaver::saveVec3(const std::string &name, const glm::tvec3<unsigned char> &vector)
    {
        file_ << std::string(indentation_, ' ') << name << " = "
            << "{" << (unsigned int)vector.x << ", " << (unsigned int)vector.y << ", "
            << (unsigned int)vector.z << "}," << std::endl;
    }

    void ModelSaver::saveVec2(const std::string &name, const glm::tvec2<unsigned short> &vector)
    {
        file_ << std::string(indentation_, ' ') << name << " = "
            << "{" << vector.x << ", " << vector.y << "}," << std::endl;
    }
}
