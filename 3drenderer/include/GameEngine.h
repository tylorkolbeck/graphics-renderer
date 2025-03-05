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
#include "Renderer.h"
#include "ImGuiWidget.h"

typedef enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method_t;

typedef enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} render_method_t;

class GameEngine
{
public:
    GameEngine(const std::string &title, int width, int height, bool full_screen);
    ~GameEngine();
    void Setup();
    void Update();
    void Render();
    void startGameLoop();
    int Init();

    const int* getWidth() { return &m_width; }
    const int* getHeight() { return &m_height; }
    const bool* isFullScreen() { return &m_full_screen; }

private:
    Window *m_window;
    std::string m_title;
    int m_width;
    int m_height;

    bool m_full_screen;
    Renderer *m_renderer;
    bool m_is_running;
    // uint32_t *m_color_buffer;
    // SDL_Texture *m_color_buffer_texture;
    vec3_t m_camera;
    cull_method_t m_cull_method = CULL_BACKFACE;
    render_method_t m_render_method = RENDER_WIRE;
    bool* m_show_grid;
    ImGuiManager *m_ImGuiManager;
    mat4_t m_proj_matrix;
    std::string m_model_file_path;
    Mesh *m_mesh;
    int m_previous_frame_time;

    ImGuiWidget *w_fpsCounter;
    ImGuiWidget *w_transform;
    ImGuiWidget *w_helloWindow;

    void processInput();
};