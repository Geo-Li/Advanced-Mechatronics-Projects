#include "wheel_encoder.h"
#include "hardware/adc.h"

uint read_light_sensor(int channel) {
    adc_select_input(channel);
    return adc_read();
}

struct encoder_info update_encoder_info(struct encoder_info e_info) {
    uint left_adc_val = read_light_sensor(LEFT_ADC_CHANNEL);
    uint right_adc_val = read_light_sensor(RIGHT_ADC_CHANNEL);
    e_info.left_adc_val = left_adc_val;
    e_info.right_adc_val = right_adc_val;
    return e_info;
}
