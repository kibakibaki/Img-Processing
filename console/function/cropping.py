import ctypes
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import time

current_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(current_dir, "../library/ImgProcessingFunction.so")
lib = ctypes.CDLL(lib_path)

# cropping
lib.cropping.restype = ctypes.POINTER(ctypes.c_ubyte)
lib.cropping.argtypes = [
    ctypes.POINTER(ctypes.c_ubyte), # imgData
    c_int, # height
    c_int, # width
    c_int, # pixelSize
    c_int, # croppingX
    c_int, # croppingY
    c_int, # targetHeight
    c_int, # targetWidth
]


def cropping(original_width, original_height, pixel_size, img_data_array):
    while True:
        try:
            croppingX = int(input("Please enter the X axis of start cropping point: "))
            if(croppingX < 0 or croppingX > original_width):
                raise ValueError
            break
        except ValueError:
            print("Invalid input. Plz enter a positive number, and within the original width")
    
    while True:
        try:
            croppingY = int(input("Please enter the Y axis of start cropping point: "))
            if(croppingY < 0 or croppingY > original_height):
                raise ValueError
            break
        except ValueError:
            print("Invalid input. Plz enter a positive number, and within the original height")
    
    remaining_width = original_width-croppingX        
    remaining_height = original_height-croppingY
    print(f"Remaining space for cropped img: width={remaining_width}, height={remaining_height}")

    while True:
        try:
            targetWidth = int(input("Plz enter a width for target width: "))
            if(targetWidth + croppingX > original_width or targetWidth <0):
                raise ValueError
            break
        except ValueError:
            print("Sorry it seems like the width is over the bound, pls enter a width that the cropping box can be inside the original img: ")

    while True:
        try:
            targetHeight = int(input("Plz enter a width for target height: "))
            if(targetHeight + croppingY > original_height or targetHeight <0):
                raise ValueError
            break
        except ValueError:
            print("Sorry it seems like the height is over the bound, pls enter a height that the cropping box can be inside the original img: ")

    # Start timing the proessing
    start_time = time.perf_counter()

    # function call
    cropped_img_size = targetWidth * targetHeight * pixel_size
    lib.cropping.restype = ctypes.POINTER(ctypes.c_ubyte * cropped_img_size)
    
    cropped_img_ptr = lib.cropping(
        img_data_array,
        original_height,
        original_width,
        pixel_size,
        croppingX,
        croppingY,
        targetHeight,
        targetWidth
    )

    cropped_img_data = bytes(cropped_img_ptr.contents)
    print(len(cropped_img_data))  # Check the size of the image data
    cropped_img = Image.frombytes('RGB', (targetWidth, targetHeight), cropped_img_data)

    # end the counting
    end_time = time.perf_counter()
    elapsed_time = end_time - start_time
    print(f"Image processing time: {elapsed_time:.6f} seconds")

    return cropped_img
