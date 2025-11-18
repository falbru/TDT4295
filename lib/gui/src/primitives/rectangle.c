#include "primitives/rectangle.h"

void renderFilledRectangle(int x, int y, int width, int height, Color color, Framebuffer *framebuffer)
{
    if (!framebuffer || !framebuffer->pixels)
        return;

    if (width <= 0 || height <= 0)
        return;

    int x_start = x < 0 ? 0 : x;
    int y_start = y < 0 ? 0 : y;
    int x_end = (x + width > framebuffer->width) ? framebuffer->width : x + width;
    int y_end = (y + height > framebuffer->height) ? framebuffer->height : y + height;

    if (x_start >= framebuffer->width || y_start >= framebuffer->height || x_end <= 0 || y_end <= 0)
        return;

    for (int row = y_start; row < y_end; row++)
    {
        for (int col = x_start; col < x_end; col++)
        {
            FRAMEBUFFER_SET_PIXEL(framebuffer, col, row, color);
        }
    }
}

void renderRectangle(int x, int y, int width, int height, Color color, int thickness, Framebuffer *framebuffer)
{
    if (!framebuffer || !framebuffer->pixels)
        return;

    if (width <= 0 || height <= 0 || thickness <= 0)
        return;

    for (int t = 0; t < thickness && t < height; t++)
    {
        renderFilledRectangle(x, y + t, width, 1, color, framebuffer);
        renderFilledRectangle(x, y + height - 1 - t, width, 1, color, framebuffer);
    }

    for (int t = 0; t < thickness && t < width; t++)
    {
        renderFilledRectangle(x + t, y, 1, height, color, framebuffer);
        renderFilledRectangle(x + width - 1 - t, y, 1, height, color, framebuffer);
    }
}
