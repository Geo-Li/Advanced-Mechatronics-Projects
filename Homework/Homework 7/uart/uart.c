#include <stdio.h>
#include "ssd1306.h"
#include "font.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"

// define general settings for initializations and operations
#define BUFFER_SIZE 50
// define charater or message boundaries for the screen
#define COL 128
#define ROW 32
#define CHAR_COL 5 // 7
#define CHAR_ROW 8 // 10
// define settings for UART
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
// define pins' functionalities on Pico
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9
#define LED_PIN PICO_DEFAULT_LED_PIN
#define PICO_I2C_SDA_PIN 12
#define PICO_I2C_SCL_PIN 13

static int chars_rxed = 0;
volatile int col = 0;
volatile int row = 0;

static void draw_char(uint x, uint y, char letter);
static void draw_string(uint x, uint y, char *message);
static void adjust_format(void);

// RX interrupt handler
void display_typed_chars()
{
    while (uart_is_readable(UART_ID))
    {
        unsigned char character = uart_getc(UART_ID);
        if (character == '\n' || character == '\r')
        {
            adjust_format();
        }
        else
        {
            if (col >= (CHAR_COL + 1) * 21)
            {
                adjust_format();
            }
            draw_char(col, row, character);
            ssd1306_update();
            col += CHAR_COL + 1;
        }
        // Send back the character to the screen
        if (uart_is_writable(UART_ID))
        {
            uart_putc(UART_ID, character);
        }
        chars_rxed++;
    }
}

int main()
{
    char message[BUFFER_SIZE];
    stdio_init_all();

    // Initialize the LED on Pico
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize SDA and SCL pins on Pico
    gpio_init(PICO_I2C_SDA_PIN);
    gpio_init(PICO_I2C_SCL_PIN);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize UART with a basic baud rate
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);
    // Set up a RX interrupt
    // We need to set up the handler first
    int UART_IRQ = UART1_IRQ;
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, display_typed_chars);
    irq_set_enabled(UART_IRQ, true);
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // Initialize I2C
    i2c_init(i2c_default, BAUD_RATE);

    // Initialize SSD1306
    ssd1306_setup();

    while (1)
    {
        // Blink the LED(GP25)
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
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

void adjust_format(void)
{
    col = 0;
    row += CHAR_ROW;
    if (row >= CHAR_ROW * 4)
    {
        row = 0;
        ssd1306_clear();
        ssd1306_update();
    }
}