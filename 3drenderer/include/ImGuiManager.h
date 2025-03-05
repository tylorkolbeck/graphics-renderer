#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include "ImGuiWidget.h"
#include <vector>

class ImGuiManager
{
public:
    ImGuiManager(SDL_Window* window, SDL_Renderer *renderer);
    ~ImGuiManager();

    void processEvent(const SDL_Event &event);
    void beginFrame();
    void endFrame();

    void addWidget(ImGuiWidget* widget);
    void renderDockingSpace();

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    std::vector<ImGuiWidget*> m_widgets;
};