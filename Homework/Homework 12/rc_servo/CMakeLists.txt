add_executable(rc_servo rc_servo.c)

# pull in common dependencies
target_link_libraries(rc_servo pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(rc_servo)
