#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define BUFFER_SIZE 100
#define ADC_RANGE 4096

int main()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    const uint BUTTON_PIN = 15;
    const uint ADC_PIN = 26;
    const char input[BUFFER_SIZE];
    const int frequency;
    uint adc_tot = 0;

    // Initialization
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }
    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    adc_init();             // init the adc module
    adc_gpio_init(ADC_PIN); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0);    // select to read from ADC0
    printf("Start!\n");

    // Loop for calculating voltage
    while (1)
    {
        // Turns on an LED
        gpio_put(LED_PIN, 1);
        // Wait until a button is pressed
        while (gpio_get(BUTTON_PIN))
        {
        }
        // Turns off the LED
        gpio_put(LED_PIN, 0);
        // Asks the user to enter a number of analog samples to take,
        // between 1 and 100
        printf("Please enter a number of analog samples between 1 and 100: \r\n");
        // Reads the number entered by the user
        scanf("%d", &frequency);
        // adc_val accumulator
        adc_tot = 0;
        // Reads the voltage on ADC0 that number of times at 100Hz
        for (int i = 0; i < frequency; i++)
        {
            // clock_t start = clock();
            uint16_t adc_val = adc_read();
            adc_tot += adc_val;
            // clock_t end = clock();
            // printf("Time elapsed: %f seconds\r\n", (double)(end - start) / CLOCKS_PER_SEC);
            sleep_ms(10);
        }
        // Prints back the voltages in the units of volts
        printf("Voltage detected: %fV\r\n", ((double)adc_tot * 3.3) / (frequency * ADC_RANGE));
    }

    return 0;
}
