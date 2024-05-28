from picamera import PiCamera
import cv2
# import numpy as np

class LineIdentifier:
    # Define global variables for the class
    iso_settings = [100,200,400,800,1600]
    
    """
        Initialize the class
    """
    def __init__(self, resolution, framerate, color=False, iso_index=3):
        self.camera = PiCamera(resolution=resolution, framerate=framerate)
        if not color:
            self.camera.color_effects = (128, 128)
            self.camera.iso = self.iso_settings[iso_index]
        else:
            pass
        
    def capture_image(self):
        self.camera.capture("line.jpg")
        image = cv2.imread("line.jpg", cv2.IMREAD_GRAYSCALE)
        return image
    
    def process_image(self, image):
        blurred_img = cv2.GaussianBlur(image, (5, 5), 0)
        _, thresholded = cv2.threshold(blurred_img, 60, 255, cv2.THRESH_BINARY_INV)
        edges = cv2.Canny(thresholded, 50, 150)
        return edges
    
    def find_line_position(self, image):
        height, width = image.shape
        roi = image[height//2:height, :]
        M = cv2.moments(roi)
        if M['m00'] == 0:
            return width // 2
        cx = int(M['m10'] / M['m00'])
        return cx
        
        
if __name__ == "__main__":
    lineIdentifier = LineIdentifier(resolution=(1920, 10280), 
                                    framerate=30, 
                                    color=False, 
                                    iso_index=4)
    lineIdentifier