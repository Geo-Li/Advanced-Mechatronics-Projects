add_executable(imu_mouse)

target_sources(imu_mouse PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/mpu6050.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

# Make sure IMU Mouse can find tusb_config.h
target_include_directories(imu_mouse PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(imu_mouse PUBLIC pico_stdlib hardware_i2c pico_unique_id tinyusb_device tinyusb_board)
pico_enable_stdio_usb(imu_mouse 1)
pico_enable_stdio_uart(imu_mouse 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(imu_mouse)
