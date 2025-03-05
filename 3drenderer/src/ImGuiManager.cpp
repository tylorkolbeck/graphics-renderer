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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // ✅ Enable docking
    io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
    io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable; // ❌ Disable multi-viewports
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

    // ✅ Ensure the Docking Space Has No Background
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0)); // Fully transparent
    renderDockingSpace();

}

void ImGuiManager::endFrame()
{
    for (ImGuiWidget *widget : m_widgets)
    {
        widget->render();
    }
    ImGui::PopStyleColor(); // ✅ Restore original background color

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void ImGuiManager::renderDockingSpace()
{
    static bool open = true;
    static bool opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // ✅ No transparency issues

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // ✅ Make Docking Window Transparent
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Fully transparent background
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpace Demo", &open, window_flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(); // Restore the original color

    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

void ImGuiManager::addWidget(ImGuiWidget *widget)
{
    m_widgets.push_back(widget);
}