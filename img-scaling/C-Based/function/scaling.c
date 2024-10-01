#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

JSAMPLE *scaling(JSAMPLE *imgData, int originalHeight, int originalWidth, int pixelSize, int newWidth, int newHeight)
{
    // Nearest Neighbor Algorithm
    JSAMPLE *resizedImgData = (JSAMPLE)malloc(newWidth * newHeight * pixelSize);
    if (resizedImgData == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }

    // calculate the ratio between new and original factrors
    float widthRatio = (float)originalWidth / newWidth;
    float heightRatio = (float)originalHeight / newHeight;

    // Loop over the newimage
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            // Find corresponding position in the original image
            int origX = (int)(x * widthRatio);
            int origY = (int)(y * heightRatio);

            // calculate posoition in the array
            int origIndex = (originalHeight * originalWidth + origX) * pixelSize;
            int newIndex = (y * newWidth + x) * pixelSize;

            // copy pixel data
            for (int i = 0; i < pixelSize; i++)
            {
                resizedImgData[newIndex + i] = imgData[origIndex + i];
            }
        }
        return resizedImgData;
    }