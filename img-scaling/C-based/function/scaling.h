#ifndef SCALING_H
#define SCALING_H

#include <jpeglib.h>

JSAMPLE *scaling(JSAMPLE *imgData, int originalHeight, int originalWidth, int pixelSize, int newWidth, int newHeight);

#endif