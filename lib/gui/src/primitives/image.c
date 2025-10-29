#include "primitives/image.h"

void renderImage(int x, int y, const Image *image, Framebuffer *framebuffer)
{
    for (int row = 0; row < image->height; row++)
    {
        if (y + row < 0 || y + row >= framebuffer->height)
            continue;

        for (int col = 0; col < image->width; col++)
        {
            if (x + col < 0 || x + col >= framebuffer->width)
                continue;

            int src_idx = (row * image->width + col);
            int dst_idx = ((y + row) * framebuffer->width + (x + col));

            framebuffer->pixels[dst_idx] = image->data[src_idx];
            framebuffer->pixels[dst_idx + 1] = image->data[src_idx + 1];
            framebuffer->pixels[dst_idx + 2] = image->data[src_idx + 2];
        }
    }
}
