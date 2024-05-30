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
        raw_capture = PiYUVArray(self.camera)
        self.camera.capture(raw_capture, format="yuv")
        image = raw_capture.array[:, :, 0]  # Extract the grayscale channel
        return image

    def process_image(self, image, threshold=128):
        # Convert image to numpy array
        image_array = np.array(image)
        
        # Apply a binary threshold to get a binary image
        binary_image = (image_array > threshold).astype(np.uint8)
        
        # Focus on the middle row of the image
        height, width = binary_image.shape
        middle_row = binary_image[height // 2, :]
        
        # Find the indices of the line in the middle row
        line_indices = np.where(middle_row == 1)[0]
        
        # Calculate the line center
        if len(line_indices) > 0:
            line_center = np.int(np.mean(line_indices))
        else:
            line_center = width // 2  # Default to center if no line is found
        
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
