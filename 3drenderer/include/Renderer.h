#pragma once
#include "Window.h"
#include "SDL2/SDL.h"
#include "vector.h"
#include "camera.h"
#include "triangles.h"
#include <vector>

class Renderer
{
public:
    Renderer(Window *window);
    ~Renderer();
    bool init();
    void drawGrid(int cellSize, uint32_t color);
    void drawDot(int cellSize, uint32_t color);
    void drawRect(int xloc, int yloc, int width, int height, uint32_t color);
    void drawRect(int xloc, int yloc, int width, int height);
    void drawPixel(int x, int y, uint32_t color);
    void createColorBuffer32(int size);
    void clearColorBuffer(uint32_t color);
    void setBufferPixel(int row, int col, uint32_t color);
    void renderColorBuffer();
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void drawLine(vec2_t a, vec2_t b, uint32_t color);
    void drawTriangle(triangle_t tri, uint32_t color);
    bool isCulled(camera camera, vec4_t vertices[]);
    bool isCulled(camera camera, vec3_t vector_a, vec3_t vector_b, vec3_t vector_c);
    SDL_Renderer *getSDLRenderer() { return m_SDL_renderer; }
    void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    uint32_t* getColorBuffer();
    void writeColorBufferToBinaryFile(const std::string &filename);

private:
    int loc_1d(int x, int y);
    void int_swap(int* a, int* b);
    void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

    Window *m_window;
    SDL_Renderer *m_SDL_renderer;
    uint32_t *m_color_buffer;
    SDL_Texture *m_color_buffer_texture;
    bool show_grid = false;
    vec3_t *p_camera;
    std::vector<triangle_t> triangles_to_render = {};

};