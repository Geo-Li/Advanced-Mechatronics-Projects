add_executable(uart uart.c ssd1306.c)

# Include the current source directory for header files
target_include_directories(uart PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# pull in common dependencies
target_link_libraries(uart pico_stdlib hardware_uart hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart)
