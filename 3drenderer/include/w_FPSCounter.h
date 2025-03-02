#pragma once
#include "ImGuiWidget.h"
#include "imgui.h"

class w_FPSCounter : public ImGuiWidget
{
public:
    void update() override
    {
    }

    void render() override
    {
        static float fps = 0.0f;
        fps = ImGui::GetIO().Framerate;

        ImGui::Begin("Performance");
        ImGui::Text("FPS: %.1f", fps);
        ImGui::End();
    }
};