import ctypes
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import time

current_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(current_dir, "../library/ImgProcessingFunction.so")
lib = ctypes.CDLL(lib_path)

# grayscale conversion
lib.convertToGrayscale.restype = POINTER(c_ubyte)
lib.convertToGrayscale.argtypes = [
    POINTER(c_ubyte), # imgData
    c_int, # width
    c_int, # height
    c_int, # pixelSize
]

def grayscale(original_width, original_height, pixel_size, img_data_array):
    # Start timing the proessing
    start_time = time.perf_counter()

    img_data_array = (ctypes.c_ubyte * len(img_data_array))(*img_data_array)

    grayscale_img_ptr = lib.convertToGrayscale(
        img_data_array,
        original_width,
        original_height,
        pixel_size
    )

    if not grayscale_img_ptr:
        raise MemoryError("Failed to allocate memory for grayscale image")

    img_size = original_width * original_height
    grayscale_img_data = bytes(grayscale_img_ptr[:img_size])
    print(f"Image data length: {len(grayscale_img_data)}")
    grayscale_img = Image.frombytes('L', (original_width, original_height), grayscale_img_data)

    # end the counting
    end_time = time.perf_counter()
    elapsed_time = end_time - start_time
    print(f"Image processing time: {elapsed_time:.6f} seconds")

    return grayscale_img 
