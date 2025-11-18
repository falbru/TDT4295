#include "primitives/image.h"

void renderImage(int x, int y, const Image *image, Framebuffer *framebuffer)
{
    for (int row = 0; row < image->height; row++)
    {
        if (y + row < 0 || y + row >= FRAMEBUFFER_HEIGHT(framebuffer))
            continue;

        for (int col = 0; col < image->width; col++)
        {
            if (x + col < 0 || x + col >= FRAMEBUFFER_WIDTH(framebuffer))
                continue;

            int src_idx = (row * image->width + col);
            int pixel_x = x + col;
            int pixel_y = y + row;

            Color src_color = image->data[src_idx];

            // Skip fully transparent pixels
            if (src_color.a == 0x00)
                continue;

            // If fully opaque, just set the pixel directly
            if (src_color.a == 0xFF)
            {
                FRAMEBUFFER_SET_PIXEL(framebuffer, pixel_x, pixel_y, src_color);
            }
            // Otherwise, blend with existing pixel
            else
            {
                Color dst_color = framebuffer->pixels[pixel_y * framebuffer->width + pixel_x];

                // Alpha blending formula: result = (src * alpha + dst * (1 - alpha)) / 255
                uint16_t alpha = src_color.a;
                uint16_t inv_alpha = 255 - alpha;

                Color blended;
                blended.a = 0xFF; // Result is fully opaque after blending
                blended.r = (src_color.r * alpha + dst_color.r * inv_alpha) / 255;
                blended.g = (src_color.g * alpha + dst_color.g * inv_alpha) / 255;
                blended.b = (src_color.b * alpha + dst_color.b * inv_alpha) / 255;

                FRAMEBUFFER_SET_PIXEL(framebuffer, pixel_x, pixel_y, blended);
            }
        }
    }
}
