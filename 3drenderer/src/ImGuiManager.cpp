#include "ImGuiManager.h"

ImGuiManager::ImGuiManager(SDL_Window *window, SDL_Renderer *renderer)
{
    m_window = window;
    m_renderer = renderer;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

    // Set up ImGui with SDL2 and SDL_Renderer
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer2_Init(m_renderer);
    
    
    // Set default font
    io.Fonts->AddFontDefault();
}

ImGuiManager::~ImGuiManager()
{
    // Cleanup ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::processEvent(const SDL_Event &event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGuiManager::beginFrame()
{
    ImGui_ImplSDL2_NewFrame();
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::endFrame()
{
    for (ImGuiWidget* widget : m_widgets)
    {
        widget->render();
    }
    
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void ImGuiManager::addWidget(ImGuiWidget* widget)
{
    m_widgets.push_back(widget);
}