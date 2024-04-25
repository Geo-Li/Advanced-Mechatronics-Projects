from serial import Serial


def main() -> int:
    serial_port = Serial(port="/dev/ttyS0", baudrate=115200, timeout=1)
    print("Serial Port Opened")
    
    while True:
        line = serial_port.readline()
        print("Read in: " + str(line) + "(" + str(type(line)) + ")")
        if len(line) > 0:
            num = float(line)
            num_str = str(num)
            print("Converted to: " + num_str + "(" + str(type(num)) + ")")
            serial_port.write((num_str+'\n\r').encode())

    return 0


if __name__ == "__main__":
    main()
