#ifndef PID_CONTROL__H__
#define PID_CONTROL__H__

extern volatile float integral;
extern volatile float previous_error;

float apply_pid_control(float error, float integral, float derivative);

#endif