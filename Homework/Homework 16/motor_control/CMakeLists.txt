add_executable(motor_control motor_control.c)

# pull in common dependencies
target_link_libraries(motor_control pico_stdlib hardware_pwm)
pico_enable_stdio_usb(motor_control 1)
pico_enable_stdio_uart(motor_control 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(motor_control)
