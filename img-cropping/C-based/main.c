#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "./function/cropping.h"
#include <time.h>

// TODO:
/*
1. import the img, read it, √
2. put data into the function √
3. time the processing, print it out √
4. enquiry about the input file name
5. start with a dummy height and width. √
6. if dummy feasible. try visualise the cropping function in dynamic ways
7. write&output the croppedImg to outputImg

*/

int main()
{
    // Enquiry for target file
    char filename[256];
    printf("Please enter the name you want to process in ImgInput file:");
    scanf("%255s", filename);
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "../ImgInput/%s", filename);

    FILE *inputFile = fopen(filepath, "rb");
    if (!inputFile)
    {
        perror("Error opening file\n");
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
    dummy data
    */
    int croppingX = 50;
    int croppingY = 100;
    int targetWidth = 200;
    int targetHeight = 150;

    /*
    Start timing the processing
    */

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    /*
    start processing the data
    */
    JSAMPLE *croppedImgData = cropping(imageData, original_height, original_width, pixelSize, croppingX, croppingY, targetHeight, targetWidth);

    if (!croppedImgData)
    {
        printf("Error: Unable to allocate memory for the resized image.\n");
        return 1;
    }

    // end of timing, start calculation of total algorithm time
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Image processing time: %.6f seconds\n", elapsed_time);

    char outputFilePath[512];
    snprintf(outputFilePath, sizeof(outputFilePath), "../ImgOutput/%s", filename);

    // output processed image
    FILE *outputFile = fopen(outputFilePath, "wb");
    if (!outputFile)
    {
        printf("Error: Could not open output file: %s\n", outputFilePath);
        free(croppedImgData);
        return 1;
    }

    // initialize the image
    struct jpeg_compress_struct cinfoOut;
    struct jpeg_error_mgr jerrOut;
    cinfoOut.err = jpeg_std_error(&jerrOut);
    jpeg_create_compress(&cinfoOut);
    jpeg_stdio_dest(&cinfoOut, outputFile);

    cinfoOut.image_width = targetWidth;
    cinfoOut.image_height = targetHeight;
    cinfoOut.input_components = pixelSize;
    cinfoOut.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfoOut);
    jpeg_start_compress(&cinfoOut, TRUE);

    // write output
    while (cinfoOut.next_scanline < cinfoOut.image_height)
    {
        row_pointer[0] = &croppedImgData[cinfoOut.next_scanline * targetWidth * pixelSize];
        jpeg_write_scanlines(&cinfoOut, row_pointer, 1);
    }

    // complete compression
    jpeg_finish_compress(&cinfoOut);
    jpeg_destroy_compress(&cinfoOut);
    fclose(outputFile);

    // clean memory
    free(croppedImgData);

    printf("Image scaling and export completed successfully.\n");
    return 0;
}