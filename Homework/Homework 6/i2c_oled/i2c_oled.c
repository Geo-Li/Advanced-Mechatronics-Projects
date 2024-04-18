#include <stdio.h>
#include "ssd1306.h"
#include "font.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// define general settings for initializations and operations
#define BUFFER_SIZE 50
#define ADC_RANGE 4096
// define charater or message boundaries for the screen
#define COL 128
#define ROW 32
#define CHAR_COL 5 // 7
#define CHAR_ROW 8 // 10
// define pins' functionalities on Pico
#define LED_PIN PICO_DEFAULT_LED_PIN
#define ADC_PIN 26
#define PICO_I2C_SDA_PIN 12
#define PICO_I2C_SCL_PIN 13

static void draw_all_pixels(void);
static void draw_char(uint x, uint y, char letter);
static void draw_string(uint x, uint y, char *message);

int main()
{
    stdio_init_all();

    // Initialize the LED on Pico
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize SDA and SCL pins on Pico
    gpio_init(PICO_I2C_SDA_PIN);
    gpio_init(PICO_I2C_SCL_PIN);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize ADC
    adc_init();             // init the adc module
    adc_gpio_init(ADC_PIN); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0);    // select to read from ADC0

    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);

    // Initialize SSD1306
    ssd1306_setup();

    char message[BUFFER_SIZE];

    while (1)
    {
        // Erase all pixels
        // ssd1306_clear();
        // ssd1306_update();

        // Blink GP25(LED)
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);

        // Count the time for reading the ADC value
        int start = to_us_since_boot(get_absolute_time());

        // Read the ADC value
        uint adc_val = adc_read();
        // Print the ADC value
        sprintf(message, "ADC voltage: %.3fV", ((double)adc_val * 3.3) / ADC_RANGE);
        draw_string(0, 0, message);
        ssd1306_update();

        // Count the time for reading the ADC value
        int end = to_us_since_boot(get_absolute_time());

        // Print the frames per second that the display achieves
        double timer = end - start;
        sprintf(message, "FPS: %.3ff/s", timer / 1000.0);
        draw_string(0, 2, message);
        ssd1306_update();

        // poll every 250ms
        sleep_ms(250 - timer);
    }

    return 0;
}

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
