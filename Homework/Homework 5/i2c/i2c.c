#include "pico/stdlib.h"
#include "hardware/i2c.h"

// device has default bus address of 0x76
#define ADDR _u(0b0100000)

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

void mcp23008_init();
void set_gp7(bool state);
bool read_gp0();

int main()
{
    stdio_init_all();
    // Initialize the LED
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);
    // Initialize SDA and SCL pins on Pico
    const uint PICO_I2C_SDA_PIN = 12;
    const uint PICO_I2C_SCL_PIN = 13;
    gpio_init(PICO_I2C_SDA_PIN);
    gpio_init(PICO_I2C_SCL_PIN);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // configure MCP23008
    mcp23008_init();
    sleep_ms(250); // sleep so that data polling and register update don't collide

    while (1)
    {
        // Blink GP25(LED)
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);

        // if the buttin in GP0 is pushed, turn on GP7(green LED),
        // else, turn off GP7(green LED)
        if (read_gp0())
        { // if gp0 is high
            set_gp7(false);
        }
        else
        { // if gp0 is low
            set_gp7(true);
        }
        // poll every 250ms
        sleep_ms(250);
    }
}

void mcp23008_init()
{
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void set_gp7(bool state)
{
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = state << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

bool read_gp0()
{
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);  // false - finished with bus
    if (buf[0] & 0b1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
