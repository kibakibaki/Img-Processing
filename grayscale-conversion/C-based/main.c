#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "./function/conversion.h"

// Driver code
int main()
{
    // setup the input file to be read
    FILE *inputFile = fopen("../inputImg/img.jpeg", "rb");
    if (!inputFile)
    {
        printf("Error opening file\n");
        return 1;
    }

    // initialise lib jpeg variables
    struct jpeg_decompress_struct cinfoIn;
    struct jpeg_error_mgr jerr;

    cinfoIn.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfoIn);

    // set source file
    jpeg_stdio_src(&cinfoIn, inputFile);

    // read file header
    jpeg_read_header(&cinfoIn, TRUE);

    // set parameters for decompression
    cinfoIn.out_color_space = JCS_RGB;
    jpeg_start_decompress(&cinfoIn);

    // get image parameters/variables:
    // width, height, pixelSize
    int width = cinfoIn.output_width;
    int height = cinfoIn.output_height;
    int pixelSize = cinfoIn.output_components;

    JSAMPLE *imageData = (JSAMPLE *)malloc(width * height * pixelSize);
    JSAMPROW rowPointer[1];
    while (cinfoIn.output_scanline < height)
    {
        rowPointer[0] = &imageData[cinfoIn.output_scanline * width * pixelSize];
        jpeg_read_scanlines(&cinfoIn, rowPointer, 1);
    }

    //******************************************************************************************* */
    // processing function call
    convertToGrayscale(imageData, width, height, pixelSize);

    //******************************************************************************************* */

    // write the output file
    FILE *outputFile = fopen("../outputImg/img.jpg", "wb");
    if (!outputFile)
    {
        printf("Error opening file\n");
        free(imageData);
        jpeg_finish_decompress(&cinfoIn);
        jpeg_destroy_decompress(&cinfoIn);
        return 1;
    }

    struct jpeg_compress_struct cinfoOut;
    struct jpeg_error_mgr jerrOut;
    cinfoOut.err = jpeg_std_error(&jerrOut);
    jpeg_create_compress(&cinfoOut);
    jpeg_stdio_dest(&cinfoOut, outputFile);

    cinfoOut.image_width = width;
    cinfoOut.image_height = height;
    cinfoOut.input_components = 1;
    cinfoOut.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfoOut);
    jpeg_start_compress(&cinfoOut, TRUE);

    while (cinfoOut.next_scanline < height)
    {
        rowPointer[0] = &imageData[cinfoOut.next_scanline * width];
        jpeg_write_scanlines(&cinfoOut, rowPointer, 1);
    }

    jpeg_finish_compress(&cinfoOut);
    jpeg_destroy_compress(&cinfoOut);
    fclose(outputFile);
    free(imageData);

    printf("Image converted successfully\n");
    return 0;
}