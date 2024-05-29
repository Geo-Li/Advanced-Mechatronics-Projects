import numpy as np
import scipy.ndimage
from picamera import PiCamera
from picamera.array import PiRGBArray

class LineIdentifier:
    # Define global variables for the class
    iso_settings = [100, 200, 400, 800, 1600]

    def __init__(self, resolution, framerate, color=False, iso_index=3):
        self.camera = PiCamera(resolution=resolution, framerate=framerate)
        if not color:
            self.camera.color_effects = (128, 128)
            self.camera.iso = self.iso_settings[iso_index]
        else:
            pass

    def capture_image(self):
        raw_capture = PiRGBArray(self.camera)
        self.camera.capture(raw_capture, format="yuv")
        image = raw_capture.array[:, :, 0]  # Extract the grayscale channel
        return image

    def process_image(self, image):
        # Apply a binary threshold to get a binary image
        threshold = 128  # You can adjust this value as needed
        binary_image = (image > threshold).astype(np.uint8)
        
        # Find the x-coordinate of the line center in the bottom half of the image
        height, width = binary_image.shape
        bottom_half = binary_image[height // 2:, :]
        column_sums = bottom_half.sum(axis=0)
        line_center = np.argmax(column_sums)
        
        # Scale the line center to a value between 0 and 20
        line_position = int((line_center / width) * 20)
        
        return line_position
    


if __name__ == "__main__":
    lineIdentifier = LineIdentifier(resolution=(640, 480), 
                                    framerate=10, 
                                    color=False, 
                                    iso_index=4)
    image = lineIdentifier.capture_image()
    line_position = lineIdentifier.process_image(image)
    print("Detected line position (0-20):", line_position)
