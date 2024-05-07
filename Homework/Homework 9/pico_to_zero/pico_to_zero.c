#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

// define general settings for initializations and operations
#define BUFFER_SIZE 50
// define settings for UART
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
// define pins' functionalities on Pico
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9

volatile int data_index = 0;
volatile char pi_zero_data[BUFFER_SIZE];

// RX interrupt handler
void get_chars()
{
    while (uart_is_readable(UART_ID))
    {
        unsigned char character = uart_getc(UART_ID);
        if (character == '\n')
        {
            // Print data on screen that is sent from Pi Zero
            pi_zero_data[data_index] = 0; // Put the null character to the end of string
            data_index = 0;
            printf("Send from Pi Zero: %s\n\r", pi_zero_data);
        }
        else
        {
            pi_zero_data[data_index] = character;
            data_index++;
        }
    }
}

int main()
{
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

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
    irq_set_exclusive_handler(UART_IRQ, get_chars);
    irq_set_enabled(UART_IRQ, true);
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    int int_from_computer;
    char input[BUFFER_SIZE];

    while (1)
    {
        // Get the data from computer
        scanf("%d", &int_from_computer);
        printf("Received from computer: %d\n\r", int_from_computer);
        // Send the data to UART
        sprintf(input, "%d\n", int_from_computer);
        uart_puts(UART_ID, input);
    }
}
