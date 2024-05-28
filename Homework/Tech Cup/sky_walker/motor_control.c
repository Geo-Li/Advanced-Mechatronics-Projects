#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define pins' functionalities on Pico
#define LEFT_PWM_PIN 6
#define RIGHT_PWM_PIN 7
#define LEFT_IO_PIN 14
#define RIGHT_IO_PIN 15
#define REVERSE_BUTTON_PIN 17
#define READ_BUTTON_PIN 5
// Define for speed control
#define LEFT_MOST_LINE 0
#define RIGHT_MOST_LINE 100

typedef struct duty_cycles
{
    int left;
    int right;
} duty_cycles;

static int line_threshold = 30;
// The PWM counters use the 125MHz system clock as a source
static const float divider = 1;               // must be between 1-255
static const short unsigned int wrap = 62500; // when to rollover, must be less than 65535
// Since the speed is related to PWM, and PWM is ranged by wrap,
// let's assign wrap to full_speed for readability
static const uint full_speed = wrap;
static volatile bool reversed_left = true;

struct duty_cycles calc_duty_cycles(int line_pos);
bool switch_state(bool state, int button_pin);

int main()
{
    // Initialization
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

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
    // Initialize the button for reread the line position
    gpio_init(READ_BUTTON_PIN);
    gpio_set_dir(READ_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(READ_BUTTON_PIN);

    printf("Start!\n");
    int line_pos;
    struct duty_cycles duty_cycles;

    while (true)
    {
        reversed_left = switch_state(reversed_left, REVERSE_BUTTON_PIN);
        if (reversed_left)
        {
            gpio_put(LEFT_IO_PIN, 1);
        }
        else
        {
            gpio_put(LEFT_IO_PIN, 0);
        }
        if (!gpio_get(READ_BUTTON_PIN))
        {
            // Asks the user to enter a number for motor control,
            // it between 1 and 100
            printf("Please enter a number for motor control (between 1 and 100): \r\n");
            // Reads the number entered by the user
            scanf("%d", &line_pos);
            duty_cycles = calc_duty_cycles(line_pos);
            printf("Detected PWM for left: %d, for right: %d\r\n", duty_cycles.left, duty_cycles.right);
            // Pass the duty cycle calculated and the digital output information to the left wheel
            if (reversed_left)
            {
                pwm_set_gpio_level(LEFT_PWM_PIN, full_speed - duty_cycles.left);
            }
            else
            {
                pwm_set_gpio_level(LEFT_PWM_PIN, duty_cycles.left);
            }
            // Pass the duty cycle calculated and the digital output information to the left wheel
            pwm_set_gpio_level(RIGHT_PWM_PIN, duty_cycles.right);
            gpio_put(RIGHT_IO_PIN, 0);
        }
    }
    return 0;
}

struct duty_cycles calc_duty_cycles(int line_pos)
{
    // For the simplest version, the controller is proportional, no derivative or integral terms.
    // But the curve will be nonlinear
    struct duty_cycles duty_cycles;
    float gradient = full_speed / line_threshold;
    if (line_pos < line_threshold)
    {
        duty_cycles.left = gradient * line_pos;
        duty_cycles.right = full_speed;
        return duty_cycles;
    }
    else if (line_pos > RIGHT_MOST_LINE - line_threshold)
    {
        duty_cycles.left = full_speed;
        duty_cycles.right = -gradient * line_pos + gradient * RIGHT_MOST_LINE;
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
