#include "Image.h"
#include <fstream>
#include <iostream>
/**
 * @brief Construct a new Image::Image object from a ppm (P6) file
 *
 * The constructor reads the ppm file and stores the pixel data in the object
 *
 *
 * @param FilePath Path to the image file
 * @throw std::runtime_error if the file cannot be opened
 * @throw std::runtime_error if the file is not a ppm file
 */
Image::Image(const char *FilePath)
{
    std::ifstream in(FilePath);

    if (!in)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    // check if file is ppm
    char buffer[3];
    in.getline(buffer, 3);
    if (buffer[0] != 'P' || buffer[1] != '6')
    {
        throw std::runtime_error("File is not a ppm file");
    }

    int width, height, maxVal;

    in >> width >> height >> maxVal;
    this->width = width;
    this->height = height;
    this->maxVal = maxVal;

    // read the rest of the file

    Pixel *pixels = new Pixel[width * height];

    for (int i = 0; i < width * height; i++)
    {
        in >> pixels[i].r >> pixels[i].g >> pixels[i].b;
    }

    this->pixels = pixels;
};

int *Image::Metadata()
{
    return new int[3]{width, height, maxVal};
};

Pixel *Image::PixelData()
{
    return pixels;
};

void Image::WriteImage(char *FileName)
{
    std::ofstream out(FileName);
    out << "P6\n"
        << width << " " << height << "\n"
        << maxVal << "\n";

    for (int i = 0; i < width * height; i++)
    {
        out << pixels[i].r << pixels[i].g << pixels[i].b;
    }
}

Image::~Image()
{
    delete[] pixels;
}