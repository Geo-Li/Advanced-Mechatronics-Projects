add_executable(i2c_oled
        i2c_oled.c
        ssd1306.c
        )

# Include the current source directory for header files
target_include_directories(i2c_oled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# pull in common dependencies
target_link_libraries(i2c_oled pico_stdlib hardware_i2c hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(i2c_oled)
