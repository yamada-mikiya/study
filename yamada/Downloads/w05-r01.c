#include <stdio.h>
#include <stdlib.h>
#include "imagedata.h"

unsigned char img[HEIGHT][WIDTH][3];
unsigned char out[HEIGHT][WIDTH][3];
unsigned char header[54];

extern int zoom(int x1, int x2, int y1, int y2);

int main(int argc, char *argv[]) {
    
    int x1 = atoi(argv[1]);
    int x2 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int y2 = atoi(argv[4]);

    read_image("apple.bmp");

    zoom(x1, x2, y1, y2);

    write_image("output.bmp");

    return 0;
}
