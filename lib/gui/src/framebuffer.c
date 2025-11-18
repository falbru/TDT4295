#include "framebuffer.h"

void framebuffer_clear(Framebuffer *framebuffer, Color clear_color)
{
    for (int i = 0; i < FRAMEBUFFER_WIDTH(framebuffer) * FRAMEBUFFER_HEIGHT(framebuffer); i++)
    {
        framebuffer->pixels[i] = clear_color;
    }
}

void framebuffer_clear_dirty_rects(Framebuffer *framebuffer, Color clear_color)
{
    for (int i = 0; i < framebuffer->dirty_rect_count; i++) {
        DirtyRect rect = framebuffer->dirty_rects[i];

        for (int y = rect.y; y < rect.y + rect.height; y++) {
            for (int x = rect.x; x < rect.x + rect.width; x++) {
                if (x >= 0 && y >= 0 && x < FRAMEBUFFER_WIDTH(framebuffer) && y < FRAMEBUFFER_HEIGHT(framebuffer))
                    FRAMEBUFFER_SET_PIXEL(framebuffer, x, y, clear_color);
            }
        }
    }

    framebuffer->dirty_rect_count = 0;
}
