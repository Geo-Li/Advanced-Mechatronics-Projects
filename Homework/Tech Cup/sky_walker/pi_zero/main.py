import time
from arducam import *
from zero_to_pico import *

def init():
    return \
        LineIdentifier(resolution = (640, 480), 
                                    framerate = 30, 
                                    color = False, 
                                    iso_index = 3),\
        ZeroToPico()

def main():
    lineIdentifier, zeroToPico = init()
    
    # pic_count = 0
    
    while True:
        image = lineIdentifier.capture_image()
        # start_time = time.time()
        line_position = lineIdentifier.process_image(image)
        # end_time = time.time()
        # print("Line position is:", line_position)
        # print("Time it takes:", (end_time - start_time))
        # print("Frame Rate:", (1/(end_time - start_time)))
        zeroToPico.sned_message(line_position)
        # pic_count += 1
        # if pic_count > 10:
        #     break
    
if __name__ == "__main__":
    main()
    
