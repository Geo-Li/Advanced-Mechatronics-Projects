#ifndef WHEEL_ENCODER__H__
#define WHEEL_ENCODER__H__

#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Define pins' functionalities on Pico
#define ADC_VARIANCE_THRESHOLD 20
#define LEFT_ADC_PIN 26
#define RIGHT_ADC_PIN 27
#define LEFT_ADC_CHANNEL 0  // ADC channel for GPIO 26
#define RIGHT_ADC_CHANNEL 1 // ADC channel for GPIO 27
#define LEFT_BRIGHT_LED_PIN 15
#define RIGHT_BRIGHT_LED_PIN 15
// Define encoder information
#define ENCODER_RESOLUTION 60

typedef struct encoder_info
{
    uint left_adc_val;
    uint right_adc_val;
} encoder_info;

extern volatile int left_encoder_count;
extern volatile int right_encoder_count;

uint read_light_sensor(int channel);
void update_encoder_count(void);

#endif
