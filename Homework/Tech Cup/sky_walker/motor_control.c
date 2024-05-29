#include "motor_control.h"

static int line_threshold = 5;
// The PWM counters use the 125MHz system clock as a source
const float divider = DIVIDER;               // must be between 1-255
const short unsigned int wrap = WRAP; // when to rollover, must be less than 65535
// Since the speed is related to PWM, and PWM is ranged by wrap,
// let's assign wrap to full_speed for readability
const uint full_speed = wrap;

void init_motor()
{
    unsigned int slice_num;
    // Set PWM for the left motor
    gpio_set_function(LEFT_PWM_PIN, GPIO_FUNC_PWM);  // Set the PWM pin
    slice_num = pwm_gpio_to_slice_num(LEFT_PWM_PIN); // Get PWM slice number
    pwm_set_clkdiv(slice_num, divider);              // Set the clock divider
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    // Initialize the GPIO pin for the left motor output
    gpio_init(LEFT_IO_PIN);
    gpio_set_dir(LEFT_IO_PIN, GPIO_OUT);
    // Set PWM for the right motor
    gpio_set_function(RIGHT_PWM_PIN, GPIO_FUNC_PWM);  // Set the PWM pin
    slice_num = pwm_gpio_to_slice_num(RIGHT_PWM_PIN); // Get PWM slice number
    pwm_set_clkdiv(slice_num, divider);               // Set the clock divider
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    // Initialize the GPIO pin for the right motor output
    gpio_init(RIGHT_IO_PIN);
    gpio_set_dir(RIGHT_IO_PIN, GPIO_OUT);

    // Initialize the button for reverse wheels
    gpio_init(REVERSE_BUTTON_PIN);
    gpio_set_dir(REVERSE_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(REVERSE_BUTTON_PIN);
}

struct motor_duty_cycles calc_duty_cycles(int line_position)
{
    // For the simplest version, the controller is proportional, no derivative or integral terms.
    // But the curve will be nonlinear
    struct motor_duty_cycles duty_cycles;
    float gradient = full_speed / line_threshold;
    if (line_position < line_threshold)
    {
        duty_cycles.left = gradient * line_position;
        duty_cycles.right = full_speed;
        return duty_cycles;
    }
    else if (line_position > RIGHT_MOST_LINE - line_threshold)
    {
        duty_cycles.left = full_speed;
        duty_cycles.right = -gradient * line_position + gradient * RIGHT_MOST_LINE;
        return duty_cycles;
    }
    else
    {
        duty_cycles.left = full_speed;
        duty_cycles.right = full_speed;
        return duty_cycles;
    }
}

bool switch_state(bool state, int button_pin)
{
    // Check if the button is pressed (active low)
    if (!gpio_get(button_pin))
    {
        // Debounce delay
        sleep_ms(50);
        // Ensure button is still pressed
        if (!gpio_get(button_pin))
        {
            // Toggle the state
            state = !state;

            // Wait until button is released
            while (!gpio_get(button_pin))
            {
                sleep_ms(10);
            }
        }
    }
    // Short delay to avoid busy-waiting
    sleep_ms(10);
    return state;
}
