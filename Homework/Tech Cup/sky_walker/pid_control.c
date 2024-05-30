#include "pid_control.h"

// PID constants
static const float Kp = 0.6;
static const float Ki = 0.0;
static const float Kd = 0.3;
// PID variables
volatile float integral = 0;
volatile float previous_error = 0;

float apply_pid_control(float error, float integral, float derivative)
{
    return Kp * error + Ki * integral + Kd * derivative;
}
