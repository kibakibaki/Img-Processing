#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

JSAMPLE *cropping(JSAMPLE *imgData, int height, int width, int pixelSize, int croppingX, int croppingY, int targetHeight, int targetWidth)
{

    if (croppingX < 0 || croppingY < 0 || croppingX + targetWidth > width || croppingY + targetHeight > height)
    {
        fprintf(stderr, "Error: Crop rectangle is out of bounds.\n");
        exit(1);
    }

    JSAMPLE *croppedImgData = (JSAMPLE *)malloc(targetHeight * targetWidth * pixelSize);
    if (croppedImgData == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }

    /*
    Algorithm below to circle the target data, put it in croppedImgData and return
    */

    // find where the x and y locates inside the image, by browsing the memory
    int actualIdx = (croppingX + croppingY * width) * pixelSize;

    for (int y = 0; y < targetHeight; y++)
    {
        // iterate the y axis in 1d memory,
        // that's: current y position + y axis of position where user picks as starting point of cropping
        int srcY = croppingY + y;
        for (int x = 0; x < targetWidth; x++)
        {
            int srcX = croppingX + x;

            int srcIndex = (srcY * width + srcX) * pixelSize;
            int destIndex = (y * targetWidth + x) * pixelSize;

            for (int i = 0; i < pixelSize; i++)
            {
                croppedImgData[destIndex + i] = imgData[srcIndex + i];
            }
        }
    }

    return croppedImgData;
};