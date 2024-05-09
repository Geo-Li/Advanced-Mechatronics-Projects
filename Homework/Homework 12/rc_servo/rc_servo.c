#include <time.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_PIN 26   // the built in LED on the Pico
#define DURATION 2   // Set the duration of the motion
#define INTERVAL 100 // Update every 100ms

static volatile short unsigned int wrap = 62500; // when to rollover, must be less than 65535

int angle_to_duty_cycle(int angle);

int main()
{
    // Initialization
    stdio_init_all();
    // Servo PWM signal has to be 50Hz
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);               // Set the LED Pin to be PWM
    unsigned int slice_num = pwm_gpio_to_slice_num(PWM_PIN); // Get PWM slice number
    float div = 40;                                          // must be between 1-255
    pwm_set_clkdiv(slice_num, div);                          // Set the clock divider
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

    int start_duty = 2.5 / 100 * wrap;
    int end_duty = 12.5 / 100 * wrap;
    int num_steps = DURATION * 1000 / INTERVAL;
    int step = (end_duty - start_duty) / num_steps;

    while (1)
    {
        // Update the position every 100ms
        int current_duty = start_duty;
        // Move from 0 to 180
        for (int i = 0; i <= num_steps; i++)
        {
            pwm_set_gpio_level(PWM_PIN, current_duty);
            sleep_ms(INTERVAL);
            current_duty += step;
        }
        // Ensure the final PWM level is set
        pwm_set_gpio_level(PWM_PIN, end_duty);
        // Move from 180 to 0
        for (int i = 0; i <= num_steps; i++)
        {
            pwm_set_gpio_level(PWM_PIN, current_duty);
            sleep_ms(INTERVAL);
            current_duty -= step;
        }
        // Ensure the final PWM level is set
        pwm_set_gpio_level(PWM_PIN, start_duty);
    }

    return 0;
}

/*
Minimum duty cycle corresponding to a 0.5ms pulse and a maximum duty cycle of 2.5ms
50Hz is 20ms, so the duty cycle can range from 2.5% to 12.5%
*/
int angle_to_duty_cycle(int angle)
{
    // Mapping 0-180 degrees to 0.5ms-2.5ms pulse width
    float pulse_width = 0.5 + (angle / 180.0) * 2.0; // Pulse width in milliseconds
    // Converting pulse width to duty cycle percentage
    return (int)((pulse_width / 20.0) * wrap);
}
