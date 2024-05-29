#ifndef MOTOR_CONTROL__H__
#define MOTOR_CONTROL__H__

#include "pico/stdlib.h"

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

typedef struct motor_duty_cycles
{
    int left;
    int right;
} motor_duty_cycles;

void init_motor();
struct motor_duty_cycles calc_duty_cycles(int line_pos);
bool switch_state(bool state, int button_pin);

#endif