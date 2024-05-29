from serial import Serial

class ZeroToPico:
    def __init__(self):
        self.serial_port = Serial(port="/dev/ttyS0", baudrate=115200, timeout=1)
        print("Serial Port Opened")
    
    """
    Send the message to Pico over the serial port
    """
    def sned_message(self, message):
        self.serial_port.write((str(message) + '\n').encode())
    

if __name__ == "__main__":
    zeroToPico = ZeroToPico()
    zeroToPico.sned_message(9)