import ctypes
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import time

current_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(current_dir, "../library/croppingFunction.so")
cropping_lib = ctypes.CDLL(lib_path)

cropping_lib.cropping.restype = ctypes.POINTER(ctypes.c_ubyte)
cropping_lib.cropping.argtypes = [
    ctypes.POINTER(ctypes.c_ubyte),
    c_int, # height
    c_int, # width
    c_int, # pixelSize
    c_int, # croppingX
    c_int, # croppingY
    c_int, # targetHeight
    c_int, # targetWidth
]

def main():
    filename = input("Please enter the name you want to process in ImgInput folder: ")
    filepath = os.path.join("../ImgInput", filename)
    if not os.path.exists(filepath):
        print("ERR: file not found.")
        return
    
    with Image.open(filepath) as img:
        img = img.convert('RGB')
        original_width, original_height = img.size
        pixel_size = 3

        img_data = img.tobytes()
        img_data_array = (ctypes.c_ubyte * len(img_data)).from_buffer_copy(img_data)

    print(f"Original image size: width={original_width}, height={original_height}")
    print("Let's start with the initial point to crop, which will be the left-up corner point of cropping box\n");

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
            print("Sorry it seems like the width is over the bound, pls enter a height that the cropping box can be inside the original img: ")

    # Start timing the proessing
    start_time = time.perf_counter()

    # function call
    cropped_img_size = targetWidth * targetHeight * pixel_size
    cropping_lib.cropping.restype = ctypes.POINTER(ctypes.c_ubyte * cropped_img_size)
    
    
        
    

if __name__ == "__main__":
    main()
