/*
This file is mainly for pin management on Pico,
all pins in this file should be covered by separate header files,
which means no file should include this file for reference.
*/
#ifndef PICO_INFO__H__
#define PICO_INFO__H__

#include "pico/stdlib.h"

// define settings for Pico
// #define BAUD_RATE 100 * 1000

// Define pins' functionalities on Pico
// For OLED screen
#define LED_PIN PICO_DEFAULT_LED_PIN
#define PICO_I2C_SDA_PIN 12
#define PICO_I2C_SCL_PIN 13
// For wheel encoder
#define BRIGHT_LED_PIN 15
#define LEFT_ADC_PIN 26
#define RIGHT_ADC_PIN 27
#define LEFT_BRIGHT_LED_PIN 15
#define RIGHT_BRIGHT_LED_PIN 15
// For motor control
#define LEFT_PWM_PIN 6
#define RIGHT_PWM_PIN 7
#define LEFT_IO_PIN 14
#define RIGHT_IO_PIN 15
#define REVERSE_BUTTON_PIN 17


#endif