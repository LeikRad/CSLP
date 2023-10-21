#include "image.h"
#include <fstream>
#include <iostream>

Image::Image(char *FileName)
{
    FILE *pFile = fopen(FileName, "rb");

    if (!pFile)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    // check if file is ppm
    char buffer[3];
    fgets(buffer, 3, pFile);
    if (buffer[0] != 'P' || buffer[1] != '6')
    {
        throw std::runtime_error("File is not a ppm file");
    }

    int width, height, maxVal;

    fscanf(pFile, "%d %d %d\n", &width, &height, &maxVal);
    this->width = width;
    this->height = height;
    this->maxVal = maxVal;

    // read the rest of the file

    Pixel *pixels = new Pixel[width * height];

    fread(pixels, sizeof(Pixel), width * height, pFile);

    this->pixels = pixels;

    fclose(pFile);
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
        << maxVal << std::endl;

    for (int i = 0; i < width * height; i++)
    {
        out << pixels[i].b << pixels[i].b << pixels[i].b;
    }
}

void Image::WriteImageWaterMark(char *FileName)
{
    // Crie uma cópia da matriz de pixels original para aplicar a marca d'água
    Pixel *watermarkedPixels = new Pixel[width * height];
    std::copy(pixels, pixels + width * height, watermarkedPixels);

    // faz um ciclo for para percorrer os últimos 10 pixels da altura e largura
    for (int i = height - 200; i < height; i++)
    {
        for (int j = width - 200; j < width; j++)
        {
            // altera os pixels para vermelho
            watermarkedPixels[i * width + j].r = 0;
            watermarkedPixels[i * width + j].g = 0;
            watermarkedPixels[i * width + j].b = 255;
        }
    }

    for (int i = height - 200; i < height; i++)
    {
        for (int j = width - 350; j < width - 200; j++)
        {
            // altera os pixels para verde
            watermarkedPixels[i * width + j].r = 0;
            watermarkedPixels[i * width + j].g = 255;
            watermarkedPixels[i * width + j].b = 0;
        }
    }

    // pinte um quadrado amarelo no meio das duas cores
    for (int i = height - 130; i < height - 70; i++)
    {
        for (int j = width - 230; j < width - 170; j++)
        {
            // altera os pixels para amarelo
            watermarkedPixels[i * width + j].r = 0;
            watermarkedPixels[i * width + j].g = 255;
            watermarkedPixels[i * width + j].b = 255;
        }
    }

    // Abra o arquivo e escreva a imagem com a marca d'água
    std::ofstream out(FileName);
    out << "P6\n"
        << width << " " << height << "\n"
        << maxVal << std::endl;

    for (int i = 0; i < width * height; i++)
    {
        out << watermarkedPixels[i].r << watermarkedPixels[i].g << watermarkedPixels[i].b;
    }

    // Libere a memória alocada para a cópia dos pixels com marca d'água
    delete[] watermarkedPixels;
}

Image::~Image()
{
    delete[] pixels;
}