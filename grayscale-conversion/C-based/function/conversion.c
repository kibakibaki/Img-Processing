#include <stdio.h>
#include <stdlib.h>
#include "conversion.h"

unsigned char *convertToGrayscale(JSAMPLE *imageData, int width, int height, int pixelSize)
{
    unsigned char *grayImageData = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (!grayImageData)
    {
        printf("Failed to allocate memory for grayscale image.\n");
        return NULL;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // the pixel data is arranged in real memory in one strip.
            // If we need to find the corresponding pixel, we need to imagine it
            // as a strip and start counting from the current row,
            // which is why we need to multiply the current height by the total width.
            // Then add the current column to find where the actual specified index really is in memory.

            // The reason for multiplying by pixelSize is that each pixel takes up
            // pixelSize bytes in memory.
            // e.g. [pixel1:[r1, g1, b1], pixel2:[r2, g2, b2], pixel3:[r3, g3, b3]]
            int index = (y * width + x) * pixelSize;

            // for this pixel, take out the red, green, and blue values
            unsigned char r = imageData[index];
            unsigned char g = imageData[index + 1];
            unsigned char b = imageData[index + 2];

            // calculation of grayscale value
            // The RGB values are converted to grayscale using the NTSC formula:
            // 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue. This formula closely represents the
            // average person's relative perception of the brightness of red, green, and blue light.

            // unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            // only red be used to create grayscale image
            // single channel grayscale image can cause loss of information
            // especially the info of brightness and contrast, and lead
            // to a very dark image

            // unsigned char gray = (unsigned char)r;

            // use average of RGB to create grayscale image
            unsigned char gray = (unsigned char)((r + g + b) / 3);

            // set the pixel to the new grayscale value
            grayImageData[y * width + x] = gray;
        }
    }
    return grayImageData;
}