#ifndef I2C_OLED__H__
#define I2C_OLED__H__

#include "pico/stdlib.h"

// define charater or message boundaries for the screen
#define COL 128
#define ROW 32
#define CHAR_COL 5 // 7
#define CHAR_ROW 8 // 10

void draw_all_pixels(void);
void draw_2x2(uint x, uint y);
void draw_char(uint x, uint y, char letter);
void draw_string(uint x, uint y, char *message);
void draw_dick(void);

#endif