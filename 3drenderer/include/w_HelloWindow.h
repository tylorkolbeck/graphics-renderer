#pragma once
#include "ImGuiWidget.h"
#include "imgui.h"

class w_HelloWindow : public ImGuiWidget
{
public:
    w_HelloWindow(bool* showGrid) : m_showGrid(showGrid)
    {}

    void update() override
    {
    }

    void render() override
    {
        ImGui::Begin("Settings");
        ImGui::Checkbox("Grid", m_showGrid);
        ImGui::End();
    }
private:
    bool* m_showGrid;
};