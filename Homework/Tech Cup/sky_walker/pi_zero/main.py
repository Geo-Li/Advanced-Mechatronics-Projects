from arducam import *
from zero_to_pico import *


def init():
    return \
        LineIdentifier(resolution = (640, 480), 
                                    framerate = 10, 
                                    color = False, 
                                    iso_index = 3),\
        ZeroToPico()

def main():
    lineIdentifier, zeroToPico = init()
    
    pic_count = 0
    
    while True:
        image = lineIdentifier.capture_image()
        line_position = lineIdentifier.process_image(image)
        print("Line position is:", line_position)
        zeroToPico.sned_message(line_position)
        pic_count += 1
        if pic_count > 10:
            break
    
    
    
if __name__ == "__main__":
    main()
    
