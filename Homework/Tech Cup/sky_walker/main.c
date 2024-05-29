// Include all header files here
#include <stdio.h>
#include <stdlib.h>
#include "ssd1306.h"
#include "i2c_oled.h"
#include "wheel_encoder.h"
#include "motor_control.h"
#include "zero_listener.h"
#include "pico_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// define general settings for initializations and operations
#define BUFFER_SIZE 50
#define BLINK_INTERVAL 100

volatile int left_wheel_count = 0;
volatile int right_wheel_count = 0;
static const uint full_speed = WRAP;
static volatile bool reversed_left = true;

int main()
{
    stdio_init_all();
    // Secure the USB connection
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    /*
    ##############################
    Initialize Pico pins
    ##############################
    */
    // Initialize the LED on Pico
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize SDA and SCL pins on Pico
    // gpio_init(PICO_I2C_SDA_PIN);
    // gpio_init(PICO_I2C_SCL_PIN);
    // gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize pins for reading light sensor
    // gpio_init(BRIGHT_LED_PIN);
    // gpio_set_dir(BRIGHT_LED_PIN, GPIO_OUT);

    /*
    ##############################
    Initialize peripherals
    ##############################
    */
    // Initialize I2C
    // i2c_init(i2c_default, BAUD_RATE);
    // // Initialize SSD1306
    // ssd1306_setup();
    // Initialize adc for reading light sensor
    adc_init();
    adc_gpio_init(LEFT_ADC_PIN);
    adc_gpio_init(RIGHT_ADC_PIN);

    init_motor();
    init_uart();

    // Initialize local variables
    char message[BUFFER_SIZE];
    struct encoder_info e_info = update_encoder_info(e_info);
    uint last_left_val;
    uint last_right_val;
    uint curr_left_val;
    uint curr_right_val;
    struct motor_duty_cycles duty_cycles;

    printf("Program Started!\n");

    while (true)
    {
        // Blink GP25(LED)
        gpio_put(LED_PIN, 1);
        sleep_ms(BLINK_INTERVAL);

        /*
        ##############################
        Start of OLED printing
        ##############################
        */
        // Print the dick!
        // sprintf(message, "FPS: %.3ff/s", timer / 1000.0);
        // draw_dick();
        // ssd1306_update();

        // Erase all pixels
        // ssd1306_clear();
        // ssd1306_update();
        /*
        ##############################
        End of OLED printing
        ##############################
        */

        /*
        ##############################
        Start of blinking the bright led and
        Read from light sensor
        ##############################
        */
        // // Turn on the LED
        // gpio_put(BRIGHT_LED_PIN, 1);

        // uint last_left_val = e_info.left_adc_val;
        // uint last_right_val = e_info.right_adc_val;

        // // Read the light sensor value
        // e_info = update_encoder_info(e_info);
        // curr_left_val = e_info.left_adc_val;
        // curr_right_val = e_info.right_adc_val;

        // // Decide if we have detected the increment of wheel position
        // if (abs(curr_left_val - last_left_val) > ADC_VARIANCE_THRESHOLD)
        // {
        //     left_wheel_count++;
        // }
        // if (abs(curr_right_val - last_right_val) > ADC_VARIANCE_THRESHOLD)
        // {
        //     right_wheel_count++;
        // }

        // printf("Left wheel position: %u\n", left_wheel_count);
        // printf("Right wheel position: %u\n", right_wheel_count);

        // // Turn off the LED
        // gpio_put(BRIGHT_LED_PIN, 0);
        /*
        ##############################
        End of blinking the bright led and
        Read from light sensor
        ##############################
        */

        /*
        ##############################
        Start of motor control
        ##############################
        */
        reversed_left = switch_state(reversed_left, REVERSE_BUTTON_PIN);
        if (reversed_left)
        {
            gpio_put(LEFT_IO_PIN, 1);
        }
        else
        {
            gpio_put(LEFT_IO_PIN, 0);
        }
        // Reads the number entered by the user
        printf("Line PositionL %d", line_position);
        duty_cycles = calc_duty_cycles(line_position);
        // printf("Detected PWM for left: %d, for right: %d\r\n", duty_cycles.left, duty_cycles.right);
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
        /*
        ##############################
        End of motor control
        ##############################
        */

        // poll every 250ms
        gpio_put(LED_PIN, 0);
        sleep_ms(BLINK_INTERVAL);
    }

    return 0;
}
