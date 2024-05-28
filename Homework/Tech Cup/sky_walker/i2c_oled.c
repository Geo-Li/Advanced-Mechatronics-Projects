#include <stdio.h>
#include "font.h"
#include "ssd1306.h"
#include "i2c_oled.h"

void draw_all_pixels(void)
{
    for (char i = 0; i < COL; i++)
    {
        for (char j = 0; j < ROW; j++)
        {
            ssd1306_drawPixel(i, j, 1);
        }
    }
}

void draw_2x2(uint x, uint y)
{
    ssd1306_drawPixel(x, y, 1);
    ssd1306_drawPixel(x + 1, y, 1);
    ssd1306_drawPixel(x, y + 1, 1);
    ssd1306_drawPixel(x + 1, y + 1, 1);
}

void draw_char(uint x, uint y, char letter)
{
    for (int col = 0; col < CHAR_COL; col++)
    {
        char chracter = ASCII[letter - 32][col];
        for (int row = 0; row < CHAR_ROW; row++)
        {
            char bit = (chracter >> row) & 0b1;
            if (bit)
            {
                ssd1306_drawPixel(x + col, y + row, 1);
            }
            else
            {
                ssd1306_drawPixel(x + col, y + row, 0);
            }
        }
    }
}

void draw_string(uint x, uint y, char *message)
{
    int index = 0;
    while (message[index])
    {
        draw_char(x + index * (CHAR_COL + 1), y * (CHAR_ROW + 1), message[index]);
        index++;
    }
}

void draw_dick(void)
{
    for (int i = 0; i <= 5; i++)
    {
        // For the up bull
        draw_2x2(10 + i, 8 - i);
        draw_2x2(24 + i, 3 + i);
        draw_2x2(10 + i, 8 + i);
        // For the buttom bull
        draw_2x2(10 + i, 24 + i);
        draw_2x2(24 + i, 29 - i);
        draw_2x2(10 + i, 24 - i);
    }

    for (int j = 16; j <= 22; j = j + 2) {
        draw_2x2(13, j);
    }

    for (int i = 12; i <= 20; i = i + 2) {
        draw_2x2(i, 3);
        draw_2x2(i, 29);
    }

    for (int i = 30; i <= 70; i = i + 2)
    {
        draw_2x2(i, 8);
        draw_2x2(i, 23);
    }
}
