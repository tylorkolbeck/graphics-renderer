#include "GameEngine.h"
#include "renderer.h"
#include "imgui.h"
#include "light.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "ImGuiManager.h"
#include "w_FPSCounter.h"
#include "w_Transform.h"
#include "w_HelloWindow.h"
#include "imgui_impl_sdlrenderer2.h"

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
    m_renderer = new Renderer(m_window);
    m_is_running = m_renderer->init();
    m_show_grid = new bool(false);

    m_ImGuiManager = new ImGuiManager(m_window->getSDLWindow(), m_renderer->getSDLRenderer());

    m_render_method = RENDER_FILL_TRIANGLE;
    m_cull_method = CULL_BACKFACE;

    float fov = M_PI / 3; // 60deg
    float aspect = (float)m_window->getHeight() / (float)m_window->getWidth();
    float znear = 0.1;
    float zfar = 100.0;
    m_proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

    m_mesh = new Mesh(m_model_file_path);
    m_mesh->translate({0.0f, 0.0f, 10.0f});

    // Register Widgets
    w_fpsCounter = new w_FPSCounter();
    w_transform = new w_Transform("Transform", m_mesh->rotation(), m_mesh->scale(), m_mesh->translation());
    w_helloWindow = new w_HelloWindow(m_show_grid);
    m_ImGuiManager->addWidget(w_fpsCounter);
    m_ImGuiManager->addWidget(w_transform);
    m_ImGuiManager->addWidget(w_helloWindow);
}

void GameEngine::Update()
{
    // int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    // if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)

    // 	SDL_Delay(time_to_wait);
    m_previous_frame_time = SDL_GetTicks();
    m_mesh->update(m_camera, m_proj_matrix, light, true, m_window);
}

void GameEngine::Render()
{
    m_ImGuiManager->beginFrame();
    if (*m_show_grid)
        m_renderer->drawGrid(100, Color::DARK_GRAY);

    m_mesh->render(m_renderer);

    m_renderer->renderColorBuffer(); // Render the color buffer to the texture
    m_renderer->clearColorBuffer(0xFF000000);

    m_ImGuiManager->endFrame();
    SDL_RenderPresent(m_renderer->getSDLRenderer());
}

void GameEngine::startGameLoop()
{
    while (m_is_running)
    {
        processInput();
        Update();
        Render();
    }
}

void GameEngine::processInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        m_ImGuiManager->processEvent(event);
        // Check if ImGui wants to capture the event (prevents SDL from handling ImGui clicks)
        ImGuiIO &io = ImGui::GetIO();

        if (io.WantCaptureMouse || io.WantCaptureKeyboard)
            continue;

        switch (event.type)
        {
        case SDL_QUIT:
            m_is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                m_is_running = false;
            // if (event.key.keysym.sym == SDLK_1)
            // 	render_method = RENDER_WIRE_VERTEX;
            // if (event.key.keysym.sym == SDLK_2)
            // 	render_method = RENDER_WIRE;
            // if (event.key.keysym.sym == SDLK_3)
            // 	render_method = RENDER_FILL_TRIANGLE;
            // if (event.key.keysym.sym == SDLK_4)
            // 	render_method = RENDER_FILL_TRIANGLE_WIRE;
            // if (event.key.keysym.sym == SDLK_c)
            // 	cull_method = CULL_BACKFACE;
            // if (event.key.keysym.sym == SDLK_d)
            // 	cull_method = CULL_NONE;
            // if (event.key.keysym.sym == SDLK_w)
            // 	camera.z += 0.1;
            // if (event.key.keysym.sym == SDLK_s)
            // 	camera.z -= 0.1;
            // if (event.key.keysym.sym == SDLK_g)
            // 	show_grid = !show_grid;
            break;
        }
    }
}

int GameEngine::Init()
{
    Setup();
    return 0;
}
