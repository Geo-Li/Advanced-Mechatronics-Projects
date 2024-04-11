add_executable(i2c
        i2c.c
        )

# pull in common dependencies
target_link_libraries(i2c pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(i2c)

