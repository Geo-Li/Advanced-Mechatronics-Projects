#include "wheel_encoder.h"

volatile int left_encoder_count = 0;
volatile int right_encoder_count = 0;
volatile int last_left_val;
volatile int last_right_val;

uint read_light_sensor(int channel)
{
    adc_select_input(channel);
    return adc_read();
}

void update_encoder_count()
{
    // Turn on LEDs
    gpio_put(LEFT_BRIGHT_LED_PIN, 1);
    gpio_put(RIGHT_BRIGHT_LED_PIN, 1);

    // Read the light sensor value
    uint left_adc_val = read_light_sensor(LEFT_ADC_CHANNEL);
    uint right_adc_val = read_light_sensor(RIGHT_ADC_CHANNEL);

    int curr_left_val = left_adc_val;
    int curr_right_val = right_adc_val;

    // Decide if we have detected the increment of wheel position
    if (abs(curr_left_val - last_left_val) > ADC_VARIANCE_THRESHOLD)
    {
        left_encoder_count++;
    }
    if (abs(curr_right_val - last_right_val) > ADC_VARIANCE_THRESHOLD)
    {
        right_encoder_count++;
    }

    printf("Left wheel position: %u\n", left_encoder_count);
    printf("Right wheel position: %u\n", right_encoder_count);

    // Turn off the LED
    gpio_put(LEFT_BRIGHT_LED_PIN, 0);
    gpio_put(RIGHT_BRIGHT_LED_PIN, 0);

    // Return adc values for next iteration
    last_left_val = left_adc_val;
    last_right_val = right_adc_val;
}
