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
        self.camera.capture(raw_capture, format="bgr")
        image = raw_capture.array
        return image

    def process_image(self, image):
        # Convert the image to grayscale
        gray_image = np.dot(image[..., :3], [0.2989, 0.5870, 0.1140])
        # Apply Gaussian blur to the image
        blurred_image = scipy.ndimage.gaussian_filter(gray_image, sigma=1.0)
        # Apply Sobel filter to find edges
        sx = scipy.ndimage.sobel(blurred_image, axis=0, mode='constant')
        sy = scipy.ndimage.sobel(blurred_image, axis=1, mode='constant')
        edges = np.hypot(sx, sy)
        edges = (edges > edges.mean()).astype(np.uint8)
        
        # Find the x-coordinate of the line center
        height, width = edges.shape
        bottom_half = edges[height // 2:, :]
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
