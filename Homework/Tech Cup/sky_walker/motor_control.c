#include "motor_control.h"

static int line_threshold = 7;
// The PWM counters use the 125MHz system clock as a source
const float divider = 1;               // must be between 1-255
const short unsigned int wrap = 62500; // when to rollover, must be less than 65535
// Since the speed is related to PWM, and PWM is ranged by wrap,
// let's assign wrap to full_speed for readability
const uint full_speed = wrap;
const uint target_speed = wrap * 0.50;
const uint left_speed = wrap * 0.5;

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

float cal_wheel_speed(int encoder_count, float time_interval)
{
    float wheel_circumference = M_PI * WHEEL_DIAMETER;
    float revolutions = (float)encoder_count / ENCODER_RESOLUTION;
    float distance = revolutions * wheel_circumference;
    float speed = distance / time_interval; // Speed in meters per second
    return speed;
}

struct motor_duty_cycles calc_duty_cycles(int line_position)
{
    // Apply the PID control to line position
    struct motor_duty_cycles duty_cycles;
    float left_speed = cal_wheel_speed(left_encoder_count, 0.2);
    float right_speed = cal_wheel_speed(right_encoder_count, 0.2);

    // We always want to center the line
    float error = line_position - (RIGHT_MOST_LINE / 2);
    integral += error;
    float derivative = error - previous_error;

    // PID output
    float output = apply_pid_control(error, integral, derivative);

    // Adjust the output based on encoder feedback
    // float speed_error_left = DESIRED_SPEED - left_speed;
    // float speed_error_right = DESIRED_SPEED - right_speed;
    // output += speed_error_left - speed_error_right;

    // Calculate the duty cycles
    duty_cycles.left = target_speed * (1 - output);
    duty_cycles.right = target_speed * (1 + output);

    // Limit duty cycles to acceptable range
    if (duty_cycles.left > target_speed)
        duty_cycles.left = target_speed;
    if (duty_cycles.left < 0)
        duty_cycles.left = 0;
    if (duty_cycles.right > target_speed)
        duty_cycles.right = target_speed;
    if (duty_cycles.right < 0)
        duty_cycles.right = 0;

    previous_error = error;
    left_encoder_count = 0;
    right_encoder_count = 0;
    return duty_cycles;
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
