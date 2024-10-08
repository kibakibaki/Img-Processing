import ctypes 
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import numpy as np
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(current_dir, "../library/libimgprocess.so")
lib = ctypes.CDLL(lib_path)

# Define the function signature from the C code
lib.grayscale.restype = POINTER(c_ubyte)
lib.grayscale.argtypes = [
    POINTER(c_ubyte), # imgData
    c_int, # width
    c_int, # height
    c_int, # pixelSize
]

def main():
    # Open the image file
    filename = input("Please enter the name you want to process in ImgInput folder: ")
    filepath = os.path.join("../ImgInput", filename)
    if not os.path.exists(filepath):
        print("Error: file not found.")
        return
    
    with Image.open(filepath) as img:   
        img = img.convert('RGB')
        original_width, original_height = img.size
        pixel_size = 3

        img_data = img.tobytes()
        img_data_array = (ctypes.c_ubyte * len(img_data)).from_buffer_copy(img_data)

    # write the output image
    grayscaled_img_ptr = lib.grayscale(
        img_data_array,
        original_width,
        original_height,
        pixel_size,
    )

    buffer_size = original_width * original_height
    grayscaled_img_array_type = ctypes.c_ubyte * buffer_size
    grayscaled_img_array = ctypes.cast(grayscaled_img_ptr, POINTER(grayscaled_img_array_type))
    grayscaled_img_data = bytes(grayscaled_img_array.contents)
    grayscaled_img = Image.frombytes('L', (original_width, original_height), grayscaled_img_data)

    output_filepath = os.path.join("../ImgOutput", filename)
    grayscaled_img.save(output_filepath, "JPEG")

    print("Image processing complete")

# Call the function
if __name__ == "__main__":
    main()