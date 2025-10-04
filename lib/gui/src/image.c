#include "image.h"
#include "asbjorn.h"

void renderImage(int x, int y, Framebuffer *framebuffer)
{
    for (int row = 0; row < ASBJORN_HEIGHT; row++)
    {
        if (y + row < 0 || y + row >= framebuffer->height)
            continue;

        for (int col = 0; col < ASBJORN_WIDTH; col++)
        {
            if (x + col < 0 || x + col >= framebuffer->width)
                continue;

            int src_idx = (row * ASBJORN_WIDTH + col);
            int dst_idx = ((y + row) * framebuffer->width + (x + col));

            framebuffer->pixels[dst_idx] = asbjorn_data[src_idx];
            framebuffer->pixels[dst_idx + 1] = asbjorn_data[src_idx + 1];
            framebuffer->pixels[dst_idx + 2] = asbjorn_data[src_idx + 2];
        }
    }
}
