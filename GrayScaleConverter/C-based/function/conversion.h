// Head guard from head file duplicated included
#ifndef CONVERSION_H
#define CONVERSION_H

#include <jpeglib.h>

void convertToGrayscale(JSAMPLE *imageData, int width, int height, int pixelSize);

#endif
