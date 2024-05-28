#ifndef WHEEL_ENCODER__H__
#define WHEEL_ENCODER__H__

#include "pico/stdlib.h"

#define ADC_VARIANCE_THRESHOLD 20
#define LEFT_ADC_PIN 26
#define RIGHT_ADC_PIN 27
#define LEFT_ADC_CHANNEL 0  // ADC channel for GPIO 26
#define RIGHT_ADC_CHANNEL 1 // ADC channel for GPIO 27

typedef struct encoder_info
{
    uint left_adc_val;
    uint right_adc_val;
} encoder_info;

uint read_light_sensor(int channel);
struct encoder_info update_encoder_info(struct encoder_info e_info);

#endif
