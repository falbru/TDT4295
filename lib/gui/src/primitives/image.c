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
            int pixel_x = x + col;
            int pixel_y = y + row;

            FRAMEBUFFER_SET_PIXEL(framebuffer, pixel_x, pixel_y, image->data[src_idx]);
        }
    }
}
