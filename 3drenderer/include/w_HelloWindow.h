#pragma once
#include "ImGuiWidget.h"
#include "imgui.h"

class w_HelloWindow : public ImGuiWidget
{
public:
    void update() override
    {
    }

    void render() override
    {
        ImGui::Begin("Hello ImGui");
        ImGui::Text("This is a test window inside SDL2!");
        ImGui::End();
    }
};