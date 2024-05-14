/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "usb_descriptors.h"
#include "mpu6050.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

// define general settings for initializations and operations
#define BUFFER_SIZE 50
// define settings for Pico
#define BAUD_RATE 100
// define pins' functionalities on Pico
#define PICO_I2C_SDA_PIN 12
#define PICO_I2C_SCL_PIN 13

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
static float deltax, deltay;
static float prev_x_gyro = 0, prev_y_gyro = 0, prev_z_gyro = 0;
static float angle_x = 0, angle_y = 0, angle_z = 0;
static float alpha = 0.1; // Low-pass filter coefficient
static float dt = 0.01;   // Time step in seconds

void led_blinking_task(void);
void hid_task(void);
float low_pass_filter(float new_data, float prev_data, float alpha);
void update_orientation(float *angle_x, float *angle_y, float *angle_z, float gx, float gy, float gz);

/*------------- MAIN -------------*/
int main(void)
{
    stdio_init_all();
    board_init();
    tusb_init();

    // Initialize SDA and SCL pins on Pico
    gpio_init(PICO_I2C_SDA_PIN);
    gpio_init(PICO_I2C_SCL_PIN);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize I2C
    i2c_init(i2c_default, BAUD_RATE);

    printf("Start!\n");

    // Initialize MPU6050
    mpu6050_init();
    // Check WHO_AM_I register
    uint8_t whoAmI = whoami();
    printf("This is the address of WHO_AM_I: %x\n", whoAmI);

    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();

        hid_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    switch (report_id)
    {
    case REPORT_ID_KEYBOARD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_keyboard_key = false;

        if (btn)
        {
            uint8_t keycode[6] = {0};
            keycode[0] = HID_KEY_A;

            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            has_keyboard_key = true;
        }
        else
        {
            // send empty key report if previously has key pressed
            if (has_keyboard_key)
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_keyboard_key = false;
        }
    }
    break;

    case REPORT_ID_MOUSE:
    {
        // Read all the data from MPU6050 and print them out
        uint8_t IMU_buf[IMU_ARRAY_LEN];
        burst_read_I2C(IMU_ADDR, ACCEL_XOUT_H, IMU_buf, IMU_ARRAY_LEN);

        // Convert the read data
        float x_acceleration = convert_accel_data(IMU_buf, 0);
        float y_acceleration = convert_accel_data(IMU_buf, 2);
        float z_acceleration = convert_accel_data(IMU_buf, 4);
        float x_gyroscope = convert_gyro_data(IMU_buf, 8);
        float y_gyroscope = convert_gyro_data(IMU_buf, 10);
        float z_gyroscope = convert_gyro_data(IMU_buf, 12);

        // Filter gyroscope data
        x_gyroscope = low_pass_filter(x_gyroscope, prev_x_gyro, alpha);
        y_gyroscope = low_pass_filter(y_gyroscope, prev_y_gyro, alpha);
        z_gyroscope = low_pass_filter(z_gyroscope, prev_z_gyro, alpha);

        update_orientation(&angle_x, &angle_y, &angle_z,
                           x_gyroscope, y_gyroscope, z_gyroscope);

        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, angle_y, angle_x, 0, 0);

        prev_x_gyro = x_gyroscope;
        prev_y_gyro = y_gyroscope;
        prev_z_gyro = z_gyroscope;
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
        // use to avoid send multiple consecutive zero report
        static bool has_consumer_key = false;

        if (btn)
        {
            // volume down
            uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
            has_consumer_key = true;
        }
        else
        {
            // send empty key report (release key) if previously has key pressed
            uint16_t empty_key = 0;
            if (has_consumer_key)
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
            has_consumer_key = false;
        }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_gamepad_key = false;

        hid_gamepad_report_t report =
            {
                .x = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0, .hat = 0, .buttons = 0};

        if (btn)
        {
            report.hat = GAMEPAD_HAT_UP;
            report.buttons = GAMEPAD_BUTTON_A;
            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

            has_gamepad_key = true;
        }
        else
        {
            report.hat = GAMEPAD_HAT_CENTERED;
            report.buttons = 0;
            if (has_gamepad_key)
                tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
            has_gamepad_key = false;
        }
    }
    break;

    default:
        break;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if (tud_suspended() && btn)
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }
    else
    {
        // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
        send_hid_report(REPORT_ID_MOUSE, btn);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;

    uint8_t next_report_id = report[0] + 1;

    if (next_report_id < REPORT_ID_COUNT)
    {
        send_hid_report(next_report_id, board_button_read());
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (!blink_interval_ms)
        return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

float low_pass_filter(float new_data, float prev_data, float alpha)
{
    return alpha * new_data + (1 - alpha) * prev_data;
}

void update_orientation(float *angle_x, float *angle_y, float *angle_z,
                        float gx, float gy, float gz)
{
    *angle_x += gx * dt;
    *angle_y += gy * dt;
    *angle_z += gz * dt;
}
