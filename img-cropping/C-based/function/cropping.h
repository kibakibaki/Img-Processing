#ifndef CROPPING_H
#define CROPPING_H

#include <jpeglib.h>

JSAMPLE *cropping(JSAMPLE *imgData, int height, int width, int pixelSize, int croppingX, int croppingY, int targetHeight, int targetWidth);

#endif