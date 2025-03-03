#pragma once

#include <string>
#include <SDL2/SDL.h>

class Window
{
public:
    Window(const std::string &title, int width, int height, bool full_screen = false);
    ~Window();
    bool init();
    void processEvents(bool &isRunning);
    SDL_Window *getSDLWindow() const { return m_window; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    void setWidth(int value);
    void setHeight(int value);

private:
    SDL_Window *m_window;
    int m_width;
    int m_height;
    std::string m_title;
    bool m_full_screen;
};