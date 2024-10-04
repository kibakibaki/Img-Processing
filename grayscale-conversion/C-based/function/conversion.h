// Head guard from head file duplicated included
#ifndef CONVERSION_H
#define CONVERSION_H

#include <jpeglib.h>

unsigned char *convertToGrayscale(JSAMPLE *imageData, int width, int height, int pixelSize);

#endif
