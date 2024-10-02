import ctypes
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import time

scaling_lib = ctypes.CDLL("../library/scalingFunction.so")

scaling_lib.scaling.restype = ctypes.POINTER(ctypes.c_ubyte)
scaling_lib.scaling.argtypes = [
    ctypes.POINTER(ctypes.c_ubyte),  # imgData
    c_int, # originalHeight
    c_int, # originalWidth
    c_int, # pixelSize
    c_int, # newWidth
    c_int #newHeight
]

def main():
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

    print(f"Original Image Size: Width={original_width} x Height={original_height}")

    while True:
        try:
            new_width = int(input("Please enter the desired new width: "))
            if(new_width <= 0):
                raise ValueError
            break
        except ValueError:
            print("Invalid input, pls enter a positive integer for width")

    while True:
        try:
            new_height = int(input("Please enter the desired new height: "))
            if(new_height <= 0):
                raise ValueError
            break
        except ValueError:
            print("Invalid input, pls enter a positive integer for height")
    
    if new_width > original_width * 10 or new_height > original_height * 10:
        print("Warning: The new dimensions are significantly larger than the original image.")

    # time counting
    start_time = time.perf_counter()

    # function call
    resized_img_size = new_width * new_height * pixel_size
    scaling_lib.scaling.restype = ctypes.POINTER(ctypes.c_ubyte * resized_img_size)

    resized_img_ptr = scaling_lib.scaling(
        img_data_array,
        original_height,
        original_width,
        pixel_size,
        new_width,
        new_height
    )

    resized_img_data = bytes(resized_img_ptr.contents)

    # end time counting
    end_time = time.perf_counter()
    elapsed_time = end_time - start_time
    print(f"Image processing time: {elapsed_time:.6f} seconds")

    resized_img = Image.frombytes('RGB', (new_width, new_height), resized_img_data)

    # save processed img
    output_filepath = os.path.join("../ImgOutput", filename)
    resized_img.save(output_filepath, "JPEG")

    print("Image scaling and export completed successfully.")
            

if __name__ == "__main__":
    main()