#include "zero_listener.h"

// define general settings for initializations and operations
#define BUFFER_SIZE 50

volatile int data_index = 0;
volatile char pi_zero_data[BUFFER_SIZE];
volatile int line_position;

// RX interrupt handler
void get_line_position()
{
    while (uart_is_readable(UART_ID))
    {
        unsigned char character = uart_getc(UART_ID);
        if (character == '\n')
        {
            // Print data on screen that is sent from Pi Zero
            pi_zero_data[data_index] = '\0'; // Put the null character to the end of string
            // Convert string to integer
            line_position = atoi(pi_zero_data);
            data_index = 0;
            // printf("Send from Pi Zero: %d\n\r", line_position);
        }
        else
        {
            pi_zero_data[data_index] = character;
            data_index++;
        }
    }
}

void init_uart()
{
    // Initialize UART with a basic baud rate
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set the data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn off FIFO's - we want to do this data by data
    uart_set_fifo_enabled(UART_ID, false);
    // Set up a RX interrupt
    // We need to set up the handler first
    int UART_IRQ = UART1_IRQ;
    // Set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, get_line_position);
    irq_set_enabled(UART_IRQ, true);
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
}
