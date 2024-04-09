add_executable(spi_dac
        spi_dac.c
        )

# pull in common dependencies
target_link_libraries(spi_dac pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi_dac)

