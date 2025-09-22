#include "primitives/text.h"

static const bdf_char_t *find_char(int encoding, const bdf_font_t *font)
{
    for (int i = 0; i < font->char_count; i++)
    {
        if (font->chars[i].encoding == encoding)
        {
            return &font->chars[i];
        }
    }
    return 0;
}

void renderText(const char *text, Color color, int x, int y, const bdf_font_t *font, Framebuffer *framebuffer)
{
    if (!text)
        return;

    int x_pos = x;

    for (int i = 0; text[i] != '\0'; i++)
    {
        const bdf_char_t *ch = find_char((int)text[i], font);
        if (!ch)
            continue;

        int storage_width_bits = ((ch->bbx_width + 7) / 8) * 8;

        for (int row = 0; row < ch->bbx_height; row++)
        {
            unsigned int row_data = ch->bitmap[row];
            int pixel_y = y + row;

            if (pixel_y < 0 || pixel_y >= FRAMEBUFFER_HEIGHT(framebuffer))
                continue;

            for (int col = 0; col < ch->bbx_width; col++)
            {
                if (row_data & (1u << (storage_width_bits - 1 - col)))
                {
                    int pixel_x = x_pos + ch->bbx_xoff + col;

                    if (pixel_x < 0 || pixel_x >= FRAMEBUFFER_WIDTH(framebuffer))
                        continue;

                    FRAMEBUFFER_SET_PIXEL(framebuffer, pixel_x, pixel_y, color);
                }
            }
        }

        x_pos += ch->width;
    }
}

int measureTextWidth(const char *text, const bdf_font_t *font)
{
    if (!text || !font)
        return 0;

    int total_width = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        const bdf_char_t *ch = find_char((int)text[i], font);
        if (ch)
        {
            total_width += ch->width;
        }
    }

    return total_width;
}

int getFontHeight(const bdf_font_t *font)
{
    if (!font || font->char_count == 0)
        return 0;

    int max_height = 0;
    for (int i = 0; i < font->char_count; i++)
    {
        int char_height = font->chars[i].bbx_height;
        if (char_height > max_height)
        {
            max_height = char_height;
        }
    }

    return max_height;
}
