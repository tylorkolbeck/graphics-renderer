#include "Renderer.h"
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_sdl2.h>
#include "color.h"
#include "triangles.h"
#include "SDL2/SDL.h"
#include <iostream>

Renderer::Renderer(Window *window) : m_window(window)
{
}

Renderer::~Renderer()
{
    // Cleanup ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    free(m_color_buffer);
    SDL_DestroyRenderer(m_SDL_renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    // ImGui_ImplSDLRenderer2_Shutdown();
    // ImGui_ImplSDL2_Shutdown();
    // ImGui::DestroyContext();
}

bool Renderer::init()
{
    m_SDL_renderer = SDL_CreateRenderer(m_window->getSDLWindow(), -1, 0);
    if (!m_SDL_renderer)
    {
        std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    createColorBuffer32(m_window->getWidth() * m_window->getHeight());

    m_color_buffer_texture = SDL_CreateTexture(
        m_SDL_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        m_window->getWidth(),
        m_window->getHeight());

    if (!m_color_buffer_texture)
    {
        std::cerr << "Error: Failed to create SDL Texture. SDL Error: " << SDL_GetError() << std::endl;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void Renderer::drawGrid(int cellSize, uint32_t color)
{
    // Draw vertical lines
    for (int x = 0; x < m_window->getWidth(); x += cellSize)
    {
        for (int y = 0; y < m_window->getHeight(); y++)
        {
            m_color_buffer[loc_1d(x, y)] = color;
        }
    }

    // Draw horizontal lines
    for (int y = 0; y < m_window->getHeight(); y += cellSize)
    {
        for (int x = 0; x < m_window->getWidth(); x++)
        {
            m_color_buffer[loc_1d(x, y)] = color;
        }
    }
}

void Renderer::drawDot(int cellSize, uint32_t color)
{
    for (int x = 0; x < m_window->getWidth(); x += cellSize)
    {
        for (int y = 0; y < m_window->getHeight(); y += cellSize)
        {
            m_color_buffer[(m_window->getWidth() * y) + x] = color;
        }
    }
}

void Renderer::drawRect(int xloc, int yloc, int width, int height, uint32_t color)
{
    for (int x = xloc; x <= xloc + width - 1; x++)
    {
        for (int y = yloc; y <= yloc + height - 1; y++)
        {
            drawPixel(x, y, color);
        }
    }
}

void Renderer::drawRect(int xloc, int yloc, int width, int height)
{
    for (int x = xloc; x <= xloc + width - 1; x++)
    {
        for (int y = yloc; y <= yloc + height - 1; y++)
        {
            drawPixel(x, y, Color::ORANGE);
        }
    }
}

void Renderer::drawPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < m_window->getWidth() && y >= 0 && y < m_window->getHeight())
    {
        m_color_buffer[loc_1d(x, y)] = color;
        // std::cout << "Drawing pixel at (" << x << ", " << y << ") with color: " << std::hex << 0xFFFF0000 << std::dec << std::endl;
    }
}

void Renderer::createColorBuffer32(int size)
{
    m_color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * size);
    if (!m_color_buffer)
    {
        std::cerr << "Error allocating memory for color buffer" << std::endl;
        return;
    }


    memset(m_color_buffer, 0, sizeof(uint32_t) * size);
    std::cout << "Buffer initialized with " << size << " pixels.\n";
    for (int i = 0; i < 10; i++)
    {
        std::cout << "Pixel " << i << ": " << std::hex << m_color_buffer[i] << std::dec << std::endl;
    }

}

void Renderer::clearColorBuffer(uint32_t color)
{
    // TODO: pass size of color buffer and buffer and set with out nested loop?
    for (int y = 0; y < m_window->getHeight(); y++)
    {
        for (int x = 0; x < m_window->getWidth(); x++)
        {
            m_color_buffer[(m_window->getWidth() * y) + x] = color;
        }
    }
}

void Renderer::setBufferPixel(int row, int col, uint32_t color)
{
    m_color_buffer[(m_window->getWidth() * row) + col] = color;
}

void Renderer::renderColorBuffer()
{
    if (!m_color_buffer)
    {
        std::cerr << "Error: Color buffer is NULL" << std::endl;
    }

    if (!m_color_buffer_texture)
    {
        std::cerr << "Error: Color buffer texture is NULL" << std::endl;
    }

    // Leave for debugging
    // int tex_width, tex_height;
    // SDL_QueryTexture(m_color_buffer_texture, NULL, NULL, &tex_width, &tex_height);

    int sdlError = SDL_UpdateTexture(
        m_color_buffer_texture,
        NULL,
        m_color_buffer,
        (int)m_window->getWidth() * sizeof(uint32_t));

    if (SDL_RenderCopy(m_SDL_renderer, m_color_buffer_texture, NULL, NULL))
    {
        std::cerr << SDL_GetError() << std::endl;
    }

    if (sdlError)
    {
        std::cerr << SDL_GetError() << std::endl;
    }

    // clearColorBuffer(0xFF000000);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int side_length = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    float x_inc = dx / (float)side_length;
    float y_inc = dy / (float)side_length;

    float current_x = x1;
    float current_y = y1;

    for (int i = 0; i <= side_length; i++)
    {
        drawPixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void Renderer::drawLine(vec2_t a, vec2_t b, uint32_t color)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int side_length = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    float x_inc = dx / (float)side_length;
    float y_inc = dy / (float)side_length;

    float current_x = a.x;
    float current_y = a.y;

    for (int i = 0; i <= side_length; i++)
    {
        drawPixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void Renderer::drawTriangle(triangle_t tri, uint32_t color)
{
    vec2_t p1 = tri.points[0];
    vec2_t p2 = tri.points[1];
    vec2_t p3 = tri.points[2];

    drawLine(p1, p2, color);
    drawLine(p2, p3, color);
    drawLine(p3, p1, color);
}

bool Renderer::isCulled(camera camera, vec4_t vertices[])
{
    vec3_t vector_a = vec3_from_vec4(vertices[0]); /* 	A	*/
    vec3_t vector_b = vec3_from_vec4(vertices[1]); /*  / \  */
    vec3_t vector_c = vec3_from_vec4(vertices[2]); /* C___B */
    // 1. Find vectors B-A and C-A
    vec3_t vector_ab = vec3_sub(vector_b, vector_a);
    vec3_t vector_ac = vec3_sub(vector_c, vector_a);
    vec3_normalize(&vector_ab);
    vec3_normalize(&vector_ac);

    // 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
    vec3_t normal = vec3_cross(vector_ab, vector_ac);
    vec3_normalize(&normal);

    // 3. Find the camera ray vector by subtracting the camera position from point A
    vec3_t camera_ray = vec3_sub({camera.location.x, camera.location.y, camera.location.z}, vector_a);
    // 4. Take the dot product between the normal N and the camera ray
    vec3_normalize(&camera_ray);
    float dot_prod = vec3_dot(normal, camera_ray);
    // 5. If this dot product is less than zero, then do not display the face

    return dot_prod < 0;
}

bool Renderer::isCulled(camera camera, vec3_t vector_a, vec3_t vector_b, vec3_t vector_c)
{
    // 1. Find vectors B-A and C-A
    vec3_t vector_ab = vec3_sub(vector_b, vector_a);
    vec3_t vector_ac = vec3_sub(vector_c, vector_a);
    vec3_normalize(&vector_ab);
    vec3_normalize(&vector_ac);

    // 2. Compute face normal using cross product of ab and ac - Order matters based on winding direction
    vec3_t normal = vec3_cross(vector_ab, vector_ac);
    vec3_normalize(&normal);

    // 3. Find the camera ray vector by subtracting the camera position from point A
    vec3_t camera_ray = vec3_sub({camera.location.x, camera.location.y, camera.location.z}, vector_a);
    vec3_normalize(&camera_ray);
    // 4. Take the dot product between the normal N and the camera ray
    float dot_prod = vec3_dot(normal, camera_ray);
    // 5. If this dot product is less than zero, then do not display the face

    return dot_prod < 0;
}

int Renderer::loc_1d(int x, int y)
{
    return (m_window->getWidth() * y) + x;
}

void Renderer::int_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void Renderer::fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Calculate slope 1 and slope 2
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0); // change in x per y, hence inverse
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0); // change in x per y, hence inverse

    // Start x_start and x_end from the top vertex
    float x_start = x0;
    float x_end = x0;
    // Loop all the scanlines
    for (int y = y0; y <= y2; y++)
    {
        // draw line between x_start and x_end
        drawLine(x_start, y, x_end, y, color);
        // based on the slope values, increments x_start and x_end for next scanline
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void Renderer::fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Calculate slope 1 and slope 2
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0); // change in x per y, hence inverse
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1); // change in x per y, hence inverse

    // Start x_start and x_end from the top vertex
    float x_start = x2;
    float x_end = x2;
    // Loop all the scanlines
    for (int y = y2; y >= y0; y--)
    {
        // draw line between x_start and x_end
        drawLine(x_start, y, x_end, y, color);
        // based on the slope values, increments x_start and x_end for next scanline
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

void Renderer::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        // Draw flat-bottom triangle
        fillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        // Draw flat-top triangle
        fillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        // Calculate the new vertex (Mx,My) using triangle similarity
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

        // Draw flat-bottom triangle
        fillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);

        // Draw flat-top triangle
        fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
    }
}

uint32_t *Renderer::getColorBuffer()
{
    return m_color_buffer;
}
#include <fstream>

void Renderer::writeColorBufferToBinaryFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Unable to open file for writing color buffer" << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char *>(m_color_buffer), m_window->getWidth() * m_window->getHeight() * sizeof(uint32_t));
    file.close();

    std::cout << "Color buffer saved as binary to " << filename << std::endl;
}

