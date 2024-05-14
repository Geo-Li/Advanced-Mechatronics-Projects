#ifndef MPU6050__H__
#define MPU6050__H__

#include "pico/stdlib.h"
#include "MPU6050_info.h"

void mpu6050_init();
uint8_t whoami(void);
uint8_t read_byte_I2C(uint8_t dev_addr, uint8_t reg_addr);
int16_t get_data(uint8_t *data, int index);
float convert_accel_data(uint8_t *data, int index);
float convert_gyro_data(uint8_t *data, int index);
void burst_read_I2C(uint8_t dev_addr, uint8_t start_reg_addr, uint8_t *data, uint8_t data_len);
void write_byte_I2C(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
void print_data(float x_acceleration, float y_acceleration, float z_acceleration, float x_gyroscope, float y_gyroscope, float z_gyroscope);

#endif