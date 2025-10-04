#include "text.h"
#include "font.h"

static const bdf_char_t *find_char(int encoding)
{
    for (int i = 0; i < FONT_CHAR_COUNT; i++)
    {
        if (font_chars[i].encoding == encoding)
        {
            return &font_chars[i];
        }
    }
    return 0;
}

void renderText(const char *text, uint8_t color, int x, int y, Framebuffer *framebuffer)
{
    if (!text)
        return;

    int x_pos = x;

    for (int i = 0; text[i] != '\0'; i++)
    {
        const bdf_char_t *ch = find_char((int)text[i]);
        if (!ch)
            continue;

        for (int row = 0; row < ch->bbx_height; row++)
        {
            unsigned int row_data = ch->bitmap[row];
            int pixel_y = y + ch->bbx_yoff + row;

            if (pixel_y < 0 || pixel_y >= framebuffer->height)
                continue;

            for (int col = 0; col < ch->bbx_width; col++)
            {
                if (row_data & (1u << (ch->bbx_width - 1 - col)))
                {
                    int pixel_x = x_pos + ch->bbx_xoff + col;

                    if (pixel_x < 0 || pixel_x >= framebuffer->width)
                        continue;

                    framebuffer->pixels[pixel_y * framebuffer->width + pixel_x] = color;
                }
            }
        }

        x_pos += ch->width;
    }
}
