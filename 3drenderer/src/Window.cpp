#include "Window.h"
#include <iostream>

Window::Window(const std::string &title, int width, int height, bool full_screen)
    : m_title(title), m_width(width), m_height(height), m_full_screen(full_screen)
{
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Failed to initialize SDL" << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS;
    SDL_DisplayMode display_mode;
    if (m_full_screen)
    {
        SDL_GetCurrentDisplayMode(0, &display_mode);
        setWidth(display_mode.w);
        setHeight(display_mode.h);
    }

    m_window = SDL_CreateWindow("SDL + ImGui",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                m_width,
                                m_height,
                                window_flags);

    SDL_SetWindowSize(m_window, m_width, m_height);
    SDL_SetWindowPosition(m_window, 0, 0);

    if (!m_window)
    {
        std::cerr << "Failed to create SDL window " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");
	SDL_SetRelativeMouseMode(SDL_FALSE);					 // Disable SDL from hijacking mouse input
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0"); // Prevent minimizing when clicking outside
	SDL_SetWindowGrab(getSDLWindow(), SDL_FALSE);					 // Ensure SDL does not "trap" mouse inside window

    return true;
}

void Window::setWidth(int value)
{
    m_width = value;
}

void Window::setHeight(int value)
{
    m_height = value;
}
