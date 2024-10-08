import ctypes
import os
from ctypes import c_int, c_float, c_ubyte, POINTER
from PIL import Image
import time
import sys

current_dir = os.path.dirname(os.path.abspath(__file__))
function_path = os.path.join(current_dir, "../function")
sys.path.append(function_path)

try:
    from grayscale import grayscale
    from scaling import scaling
    from cropping import cropping
except ImportError:
    print("ERR: module not found.")
    sys.exit(1)

# # define the functions
# # scaling
# lib.scaling.restype = ctypes.POINTER(ctypes.c_ubyte)
# lib.scaling.argtypes = [
#     ctypes.POINTER(ctypes.c_ubyte),  # imgData
#     c_int, # originalHeight
#     c_int, # originalWidth
#     c_int, # pixelSize
#     c_int, # newWidth
#     c_int #newHeight
# ]

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
    
    
    while True:            
        print("1 grayscale conversion")
        print("2 cropping")
        print("3 scaling")
        choice = input("Please enter a number to select the function you want to use: ")

        if choice == "1":
            print("start grayscale conversion...")  
            grayscale_img = grayscale(original_width, original_height, pixel_size, img_data_array)    
            function_number = 1
            break
        elif choice == "2":
            print("start cropping...")
            print(f"Original image size: width={original_width}, height={original_height}")
            print("Let's start with the initial point to crop, which will be the left-up corner point of cropping box\n");
            cropped_img = cropping(original_width, original_height, pixel_size, img_data_array)
            function_number = 2
            break
        elif choice == "3":
            print("start scaling...")
            print(f"Original image size: width={original_width}, height={original_height}")
            scaling_img = scaling(original_width, original_height, pixel_size, img_data_array)
            function_number = 3
            break
        else:
            print("invalid choice, please enter a valid input.\n")
    
    # save processed img
    output_filepath = os.path.join("../ImgOutput", filename)
    if function_number == 1:
        grayscale_img.save(output_filepath, "JPEG")
    elif function_number ==2:
        cropped_img.save(output_filepath, "JPEG")
    elif function_number ==3:
        scaling_img.save(output_filepath, "JPEG")


    print("Image scaling and export completed successfully.")
  



if __name__ == "__main__":
    main()
