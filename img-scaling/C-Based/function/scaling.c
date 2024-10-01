#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

unsigned char *scaling(JSAMPLE *imgData, int orioginalHeight, int originalWidth, int pixelSize, int newWidth, int newHeight)
{
    // Nearest Neighbor Algorithm
    JSAMPLE *resizedImgData = (JSAMPLE)malloc(newWidth * newHeight * pixelSize);
    if (resizedImgData == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }

    return resizedImgData;
}