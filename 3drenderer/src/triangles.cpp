#include "triangles.h"
#include "vector.h"
#include "display.h"
#include <cstdint>

void int_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void fill_flat_bottom_triangle(uint32_t *buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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
        draw_line(buffer_p, x_start, y, x_end, y, color);
        // based on the slope values, increments x_start and x_end for next scanline
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void fill_flat_top_triangle(uint32_t *buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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
        draw_line(buffer_p, x_start, y, x_end, y, color);
        // based on the slope values, increments x_start and x_end for next scanline
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

// TODO: Fix this function, the vector swap messes things up
// void sort_vertices_by_y(vec2_t &v0, vec2_t &v1, vec2_t v2)
// {
//     if (v0.y > v1.y)
//     {
//         std::swap(v0, v1);
//     }
//     if (v1.y > v2.y)
//     {
//         std::swap(v1, v2);
//     }
//     if (v0.y > v2.y)
//     {
//         std::swap(v0, v2);
//     }
// }

// TODO: Fix this function, the vector swap messes things up
// void draw_filled_triangle(uint32_t *buffer_p, triangle tri, uint32_t color)
// {
//     // Sort the vertices
//     vec2_t v0 = tri.points[0];
//     vec2_t v1 = tri.points[1];
//     vec2_t v2 = tri.points[2];

//     sort_vertices_by_y(v0, v1, v2);

//     if (v1.y == v2.y)
//     {
//         fill_flat_bottom_triangle(buffer_p, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, 0xFF00FF00);
//     }
//     else if (v0.y == v1.y)
//     {
//         fill_flat_top_triangle(buffer_p, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, 0xFFFF0000);
//     }
//     else
//     {
//         // Find the midpoint
//         vec2_t md{};
//         md.x = ((float)((v2.x - v0.x) * (v1.y - v0.y)) / (float)(v2.y - v0.y)) + v0.x;
//         md.y = v1.y;
//         fill_flat_bottom_triangle(buffer_p, v0.x, v0.y, v1.x, v1.y, md.x, md.y, 0xFF00FF00);
//         fill_flat_top_triangle(buffer_p, v1.x, v1.y, md.x, md.y, v2.x, v2.y, 0xFFFF0000);
//     }
// }

void draw_filled_triangle(uint32_t *buffer_p, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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
        fill_flat_bottom_triangle(buffer_p, x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        // Draw flat-top triangle
        fill_flat_top_triangle(buffer_p, x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        // Calculate the new vertex (Mx,My) using triangle similarity
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(buffer_p, x0, y0, x1, y1, Mx, My, color);

        // Draw flat-top triangle
        fill_flat_top_triangle(buffer_p, x1, y1, Mx, My, x2, y2, color);
    }
}