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
        # Convert image to numpy array
        image_array = np.array(image)
        
        # Apply a binary threshold to get a binary image
        binary_image = (image_array > threshold).astype(np.uint8)
        
        # Focus on the bottom half of the image
        height, width = binary_image.shape
        bottom_half = binary_image[height // 2:, :]
        
        # Initialize sliding window parameters
        window_height = 20
        num_windows = height // (2 * window_height)
        margin = 25
        minpix = 50
        
        # Find the starting point of the line
        histogram = np.sum(bottom_half, axis=0)
        midpoint = np.int(histogram.shape[0] // 2)
        leftx_base = np.argmax(histogram[:midpoint])
        rightx_base = np.argmax(histogram[midpoint:]) + midpoint
        
        # Current positions to be updated for each window
        leftx_current = leftx_base
        rightx_current = rightx_base
        
        # Create empty lists to receive left and right lane pixel indices
        left_lane_inds = []
        right_lane_inds = []
        
        for window in range(num_windows):
            # Identify window boundaries in x and y
            win_y_low = height - (window + 1) * window_height
            win_y_high = height - window * window_height
            win_xleft_low = leftx_current - margin
            win_xleft_high = leftx_current + margin
            win_xright_low = rightx_current - margin
            win_xright_high = rightx_current + margin
            
            # Identify the nonzero pixels in x and y within the window
            good_left_inds = ((bottom_half[win_y_low:win_y_high, win_xleft_low:win_xleft_high]).nonzero()[0])
            good_right_inds = ((bottom_half[win_y_low:win_y_high, win_xright_low:win_xright_high]).nonzero()[0])
            
            # Append these indices to the lists
            left_lane_inds.append(good_left_inds)
            right_lane_inds.append(good_right_inds)
            
            # If you found > minpix pixels, recenter next window on their mean position
            if len(good_left_inds) > minpix:
                leftx_current = np.int(np.mean(good_left_inds))
            if len(good_right_inds) > minpix:
                rightx_current = np.int(np.mean(good_right_inds))
        
        # Concatenate the arrays of indices
        left_lane_inds = np.concatenate(left_lane_inds)
        right_lane_inds = np.concatenate(right_lane_inds)
        
        # Calculate the line center
        if len(left_lane_inds) > 0:
            left_center = np.int(np.mean(left_lane_inds))
        else:
            left_center = width // 4  # Default to left quarter
        
        if len(right_lane_inds) > 0:
            right_center = np.int(np.mean(right_lane_inds))
        else:
            right_center = 3 * width // 4  # Default to right quarter
        
        line_center = (left_center + right_center) // 2
        
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