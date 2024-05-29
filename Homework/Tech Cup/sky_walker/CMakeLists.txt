# Set the project name
project(SKY_WALKER)

add_executable(sky_walker
    main.c
    ssd1306.c
    i2c_oled.c
    wheel_encoder.c
    motor_control.c
    zero_listener.c
)

# Include the current source directory for header files
target_include_directories(sky_walker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Pull in common dependencies
target_link_libraries(sky_walker
    pico_stdlib
    hardware_i2c
    hardware_adc
    hardware_pwm
    hardware_uart
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(sky_walker 1)
pico_enable_stdio_uart(sky_walker 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(sky_walker)
