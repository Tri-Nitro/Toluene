#ifndef TOLUENE_IMAGE_H
#define TOLUENE_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tiled_tiff_decoder(int* data, int image_length, int image_width,
                        int tile_length, int tile_width, int bytes_per_channel,
                        int color_depth, int* outdata);

#endif //TOLUENE_IMAGE_H
