from gpiozero import LED, Button


def main(LED_pin, Button_Pin):
    # Initialize LED and Button on Pi Zero
    led = LED(LED_Pin)
    button = Button(Button_Pin)
    while True:
        button.when_pressed = led.toggle
    

if __name__ == "__main__":
    LED_Pin = 16; Button_Pin = 20
    main(LED_Pin, Button_Pin)

