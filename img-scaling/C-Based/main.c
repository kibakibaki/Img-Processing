#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "./function/scaling.h"

// Driver code
int main()
{
    FILE *inputFile = fopen("../ImgInput/img.jpg", "rb");
    if (!inputFile)
    {
        printf("Error opening file\n");
        return 1;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, inputFile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int original_width = cinfo.output_width;
    int original_height = cinfo.output_height;
    int pixelSize = cinfo.output_components;

    JSAMPLE *imageData = (JSAMPLE *)malloc(original_width * original_height * pixelSize);
    JSAMPROW row_pointer[1];

    while (cinfo.output_scanline < cinfo.output_height)
    {
        row_pointer[0] = &imageData[cinfo.output_scanline * original_width * pixelSize];
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(inputFile);

    /*

    Calling the scaling transformation function

    */

    int new_width = original_width / 2;   // Example: shrink to half
    int new_height = original_height / 2; // Example: shrink to half

    JSAMPLE *resizedImage = scaling(imageData, original_height, original_width, pixelSize, new_width, new_height);

    /*

    End of scaling transformation

    */

    if (!resizedImage)
    {
        printf("Error: Unable to allocate memory for the resized image.\n");
        return 1;
    }

    // output processed image
    FILE *outputFile = fopen("../ImgOutput/resized_image.jpg", "wb");
    if (!outputFile)
    {
        printf("Error: Could not open output file.\n");
        free(resizedImage);
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return 1;
    }

    // initialize the image
    struct jpeg_compress_struct cinfoOut;
    struct jpeg_error_mgr jerrOut;
    cinfoOut.err = jpeg_std_error(&jerrOut);
    jpeg_create_compress(&cinfoOut);
    jpeg_stdio_dest(&cinfoOut, outputFile);

    cinfoOut.image_width = new_width;
    cinfoOut.image_height = new_height;
    cinfoOut.input_components = pixelSize;
    cinfoOut.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfoOut);
    jpeg_start_compress(&cinfoOut, TRUE);

    // write output
    while (cinfoOut.next_scanline < cinfoOut.image_height)
    {
        row_pointer[0] = &resizedImage[cinfoOut.next_scanline * new_width * pixelSize];
        jpeg_write_scanlines(&cinfoOut, row_pointer, 1);
    }

    // complete compression
    jpeg_finish_compress(&cinfoOut);
    jpeg_destroy_compress(&cinfoOut);
    fclose(outputFile);

    // clean memory
    free(resizedImage);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(inputFile);

    printf("Image scaling and export completed successfully.\n");
    return 0;
}
