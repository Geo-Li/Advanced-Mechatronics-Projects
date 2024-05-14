#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "MPU6050.h"

void mpu6050_init()
{
    write_byte_I2C(IMU_ADDR, PWR_MGMT_1, 0b00);
    write_byte_I2C(IMU_ADDR, ACCEL_CONFIG, 0b00);
    write_byte_I2C(IMU_ADDR, GYRO_CONFIG, 0b11);
}

uint8_t whoami(void)
{
    return read_byte_I2C(IMU_ADDR, WHO_AM_I);
}

uint8_t read_byte_I2C(uint8_t dev_addr,
                      uint8_t reg_addr)
{
    uint8_t answer;
    i2c_write_blocking(i2c_default, IMU_ADDR, &reg_addr, 1, true);
    i2c_read_blocking(i2c_default, IMU_ADDR, &answer, 1, false);
    return answer;
}

int16_t get_data(uint8_t *data, int index)
{
    return data[index] << 8 | data[index + 1];
}

float convert_accel_data(uint8_t *data, int index)
{
    return get_data(data, index) * 0.000061;
}

float convert_gyro_data(uint8_t *data, int index)
{
    return get_data(data, index) * 0.007630;
}

void burst_read_I2C(uint8_t dev_addr,
                    uint8_t start_reg_addr,
                    uint8_t *data,
                    uint8_t data_len)
{
    i2c_write_blocking(i2c_default, dev_addr, &start_reg_addr, 1, true); // Send register address
    i2c_read_blocking(i2c_default, dev_addr, data, data_len, false);     // Read data
}

void write_byte_I2C(uint8_t dev_addr,
                    uint8_t reg_addr,
                    uint8_t data)
{
    uint8_t buf[2];
    // send register number followed by its corresponding value
    buf[0] = reg_addr;
    buf[1] = data;
    i2c_write_blocking(i2c_default, dev_addr, buf, 2, false);
}

void print_data(float x_acceleration, float y_acceleration, float z_acceleration,
                float x_gyroscope, float y_gyroscope, float z_gyroscope)
{
    // Print the read data
    printf("The acceleration data:\n"
           "x: %f\n"
           "y: %f\n"
           "z: %f\n"
           "The gyroscope data:\n"
           "x: %f\n"
           "y: %f\n"
           "z: %f\n",
           x_acceleration, y_acceleration, z_acceleration,
           x_gyroscope, y_gyroscope, z_gyroscope);
}
