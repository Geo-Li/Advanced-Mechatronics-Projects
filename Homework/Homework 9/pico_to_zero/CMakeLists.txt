add_executable(pico_to_zero pico_to_zero.c)

# pull in common dependencies
target_link_libraries(pico_to_zero pico_stdlib hardware_uart)
pico_enable_stdio_usb(pico_to_zero 1)
pico_enable_stdio_uart(pico_to_zero 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_to_zero)
