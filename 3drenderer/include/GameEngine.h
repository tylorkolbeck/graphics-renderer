#pragma once
#include "Window.h"
#include "SDL2/SDL.h"
#include "vector.h"
#include "renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "ImGuiManager.h"
#include "matrix.h"
#include "Mesh.h"

class GameEngine
{
public:
    GameEngine(const std::string &title, int width, int height, bool full_screen);
    ~GameEngine();
    void Setup();
    void Update();
    void Render();
    int Init();

    const int* getWidth() { return &m_width; }
    const int* getHeight() { return &m_height; }
    const bool isFullScreen() { return &m_full_screen; }

private:
    Window *m_window;
    std::string m_title;
    int m_width;
    int m_height;

    bool m_full_screen;
    SDL_Renderer *m_renderer;
    bool m_is_running;
    uint32_t *m_color_buffer;
    SDL_Texture *m_color_buffer_texture;
    vec3_t m_camera;
    cull_method_t m_cull_method = CULL_BACKFACE;
    render_method_t m_render_method = RENDER_WIRE;
    vec3_t m_show_grid;
    ImGuiManager *m_ImGuiManager;
    mat4_t m_proj_matrix;
    std::string m_model_file_path;
    Mesh m_mesh{};
    int m_previous_frame_time;
};