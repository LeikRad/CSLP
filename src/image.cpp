#include "image.h"
#include <fstream>
#include <iostream>
using namespace std;

using namespace std;
Image::Image(FILE* pFile) {
    // check if file is ppm
    char buffer[3];
    fgets(buffer, 3, pFile);
    if (buffer[0] != 'P' || buffer[1] != '6') {
        throw std::runtime_error("File is not a ppm file");
    }

    int width, height, maxVal;

    fscanf(pFile, "%d %d %d", &width, &height, &maxVal);
    this->width = width;
    this->height = height;
    this->maxVal = maxVal;

    // read the rest of the file 

    Pixel *pixels = new Pixel[width * height];

    fread(pixels, sizeof(Pixel), width * height, pFile);

    this->pixels = pixels;
};

int* Image::Metadata(){
    return new int[3]{width, height, maxVal};
};

Pixel* Image::PixelData(){ 
    return pixels;
};

void Image::WriteImage(char *filename) {
    ofstream out(filename);
    out << "P6\n" << width << " " << height << "\n" << maxVal;

    for( int i = 0; i < width * height; i++) {
        out << pixels[i].r << pixels[i].g << pixels[i].b;
    }
}

Image::~Image() {
    delete[] pixels;
}