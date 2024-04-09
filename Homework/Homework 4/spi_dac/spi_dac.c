#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
// #include "pico/binary_info.h"
#include "hardware/spi.h"

static uint sine_wave[50];
static uint triangle_wave[100];

void generate_waves(bool wave_flag);
static inline void cs_select();
static inline void cs_deselect();
static void write_register(uint info);

int main()
{
    stdio_init_all();
    // Use SPI0 at 12k Hz
    spi_init(spi_default, 12 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    generate_waves(0); // makes a 2Hz sine wave
    generate_waves(1); // makes a 1Hz triangle wave
    static uint analog = 0;

    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            analog = 0;
            analog |= triangle_wave[i] << 2;
            analog |= 0b0111 << 12; // set up the setting bits
            write_register(analog);
            analog = 0;
            if (i < 50)
            {
                analog |= sine_wave[i] << 2;
            }
            else
            {
                analog |= sine_wave[i - 50] << 2;
            }
            analog |= 0b1111 << 12; // set up the setting bits
            write_register(analog);
            sleep_ms(10);
        }
    }

    return 0;
}

void generate_waves(bool wave_flag)
{
    uint vol_high = 1023;
    uint vol_low = 0;
    if (!wave_flag)
    {
        for (int i = 0; i < 50; i++)
        {
            float radians = sin(M_PI * 2 * ((float)i / 49));
            sine_wave[i] = (radians + 1) * vol_high / 2;
        }
    }
    else
    {
        uint step = vol_high / 50;
        for (int i = 0; i < 50; i++)
        {
            triangle_wave[i] = step * i;
        }
        for (int i = 0; i < 50; i++)
        {
            triangle_wave[50 + i] = vol_high - step * i;
        }
    }
}

static inline void cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0); // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

static void write_register(uint info)
{
    uint8_t buf[2];
    buf[0] = info >> 8;
    buf[1] = info;

    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
}
