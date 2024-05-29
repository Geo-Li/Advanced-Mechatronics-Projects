#ifndef ZERO_LISTENER__H__
#define ZERO_LISTENER__H__

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

// define settings for UART
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
// define pins' functionalities on Pico
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9

extern volatile int line_position;

void get_line_position();
void init_uart();

#endif