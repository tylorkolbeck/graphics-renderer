#pragma once
#include "ImGuiWidget.h"
#include "vector.h"
#include "imgui.h"
#include <string>

class w_Transform : public ImGuiWidget
{
public:
    w_Transform(const std::string& label, vec3_t& rotation, vec3_t& scale, vec3_t& position) 
        : m_label(label), m_rotation(rotation), m_scale(scale), m_position(position)
    {
    }

    void update() override
    {
    }

    void render() override
    {
        ImGui::Begin(m_label.c_str());

        ImGui::Text("Rotation");
        ImGui::DragFloat("X", &m_rotation.x, 0.1f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat("Y##2", &m_rotation.y, 0.1f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat("Z##3", &m_rotation.z, 0.1f, 0.0f, 0.0f, "%.3f");

        ImGui::Separator();
    
        ImGui::Text("Scale");
        ImGui::Checkbox("Uniform Scaling", &m_uniform_scale);
        if (m_uniform_scale)
        {
            static float uniform_scale = 1.0f;
            if (ImGui::DragFloat("Scale", &uniform_scale, 0.1f, 0.0f, 0.0f, "%.3f"))
            {
                m_scale.x = m_scale.y = m_scale.z = uniform_scale;
            }
        }
        else
        {
            ImGui::DragFloat("X##4", &m_scale.x, 0.1f, 0.0f, 0.0f, "%.3f");
            ImGui::DragFloat("X##5", &m_scale.y, 0.1f, 0.0f, 0.0f, "%.3f");
            ImGui::DragFloat("X##6", &m_scale.z, 0.1f, 0.0f, 0.0f, "%.3f");
        }

        ImGui::Text("Position");
        ImGui::DragFloat("X##7", &m_position.x, 0.1f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat("X##8", &m_position.y, 0.1f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat("X##9", &m_position.z, 0.1f, 0.0f, 0.0f, "%.3f");

        ImGui::End();
    }

private:
    const std::string m_label;
    vec3_t &m_rotation;
    vec3_t &m_scale;
    vec3_t &m_position;
    bool m_uniform_scale = true;

};