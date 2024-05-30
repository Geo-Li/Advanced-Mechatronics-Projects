import numpy as np
from PIL import Image
from picamera import PiCamera
from picamera.array import PiYUVArray

class LineIdentifier:
    # Define global variables for the class
    iso_settings = [100, 200, 400, 800, 1600]

    def __init__(self, resolution, framerate, color = False, iso_index = 3):
        self.camera = PiCamera(resolution=resolution, framerate=framerate)
        if not color:
            self.camera.color_effects = (128, 128)
            self.camera.iso = self.iso_settings[iso_index]
        else:
            pass

    def capture_image(self):
        self.camera.capture("image.jpg")
        image = Image.open("image.jpg").convert('L')  # Convert to grayscale
        return image

    def process_image(self, image, threshold = 128):
        # Apply a binary threshold to get a binary image
        binary_image = (np.array(image) > threshold).astype(np.uint8)
        
        # Find the x-coordinate of the line center in the bottom half of the image
        height, width = binary_image.shape
        bottom_half = binary_image[height // 2:, :]
        column_sums = bottom_half.sum(axis=0)
        line_center = np.argmax(column_sums)
        
        # Scale the line center to a value between 0 and 20
        line_position = int((line_center / width) * 20)
        
        return line_position
    


if __name__ == "__main__":
    lineIdentifier = LineIdentifier(resolution = (640, 480), 
                                    framerate = 10, 
                                    color = False, 
                                    iso_index = 3)
    image = lineIdentifier.capture_image()
    line_position = lineIdentifier.process_image(image)
    print("Detected line position (0-20):", line_position)
