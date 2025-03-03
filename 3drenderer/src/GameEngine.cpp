#include "GameEngine.h"
#include "renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "ImGuiManager.h"

GameEngine::GameEngine(const std::string &title, int width, int height, bool full_screen)
    : m_title(title), m_width(width), m_height(height), m_full_screen(full_screen)
{
}

GameEngine::~GameEngine()
{

}

void GameEngine::Setup()
{
    m_model_file_path = "assets/f22.obj";
    m_window = new Window(m_title, m_width, m_height, m_full_screen);
    m_is_running = m_window->init();
    m_is_running = initialize_renderer(m_window);

    m_ImGuiManager = new ImGuiManager(m_window->getSDLWindow(), renderer);

    m_render_method = RENDER_FILL_TRIANGLE;
    m_cull_method = CULL_BACKFACE;

    // Creating SDL texture used to display color buffer
    m_color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    float fov = M_PI / 3; // 60deg
    float aspect = (float)m_window->getHeight() / (float)m_window->getWidth();
    float znear = 0.1;
    float zfar = 100.0;
    m_proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

    m_mesh = Mesh(m_model_file_path);
    m_mesh.translate({0.0f, 0.0f, 10.0f});
    // Register Widgets
    // w_helloWindow = new w_HelloWindow();
    // w_fpsCounter = new w_FPSCounter();
    // w_transform = new w_Transform("Transform", m_mesh.rotation(), m_mesh.scale(), m_mesh.translation());

    // imguiManager->addWidget(w_helloWindow);
    // imguiManager->addWidget(w_fpsCounter);
    // imguiManager->addWidget(w_transform);
}

void GameEngine::Update()
{
    // int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    // if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)

    // 	SDL_Delay(time_to_wait);
    m_previous_frame_time = SDL_GetTicks();
}

void GameEngine::Render()
{
    m_ImGuiManager->beginFrame();

    // // Show a simple test window
    // ImGui::Begin("Hello ImGui");
    // ImGui::Text("This is a test window inside SDL2!");
    // ImGui::End();

    // if (show_grid)
    //     draw_grid(color_buffer, 10, Color::C_LIGHTGREY);

    // Draw the model
    m_mesh.render(m_color_buffer);

    render_color_buffer();          // Render the color buffer to the texture
    clear_color_buffer(0xFF000000); // Set each pixel to yellow color in buffer

    m_ImGuiManager->endFrame();
    SDL_RenderPresent(renderer);
}

int GameEngine::Init()
{
    Setup();

    while (m_is_running)
    {
        // process_input();
        Update();
        Render();
    }

    delete m_ImGuiManager;
    destroy_renderer();

    return 0;
}
