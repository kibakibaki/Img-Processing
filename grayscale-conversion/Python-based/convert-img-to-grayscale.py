import ctypes 
from PIL import Image
import numpy as np

lib_path = "../library/libimgprocess.so"
lib = ctypes.CDLL(lib_path)

# Define the function signature from the C code
# convertToGrayscale(JSAMPLE *imageData, int width, int height, int pixelSize);
lib.convertToGrayscale.argtypes = (ctypes.POINTER(ctypes.c_ubyte), ctypes.c_int, ctypes.c_int, ctypes.c_int)
lib.convertToGrayscale.restype = None

def convert_img_to_grayscale(input_path, output_path):
    # Open the image file
    img = Image.open(input_path)
    img = img.convert("RGB")

    # pit the image into a numpy array
    image_array = np.array(img, dtype=np.uint8)
    height, width, pixel_size = image_array.shape

    # convert the img to ctypes array
    image_data = image_array.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))

    #use C function
    lib.convertToGrayscale(image_data, width, height, pixel_size)

    # write the output image
    gray_image_array = np.ctypeslib.as_array(image_data, shape=(height, width)) 
    gray_img = Image.fromarray(gray_image_array, mode='L')

    gray_img.save(output_path)
    print("Image processing complete")

# Call the function
convert_img_to_grayscale("../inputImg/img.jpeg", "../outputImg/img_gray.jpg")