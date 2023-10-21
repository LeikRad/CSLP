#include <iostream>
struct Pixel
{
    u_char r;
    u_char g;
    u_char b;
};

class Image
{
private:
    int width;
    int height;
    int maxVal;
    Pixel *pixels; // Pointer to a array (which is a pointer in itself) so pointer to pointer

public:
    Image(const char *FileName);
    ~Image();
    int *Metadata();
    Pixel *PixelData();
    void WriteImage(const char *FileName);
    void WriteImageWaterMark(Image &watermarkImage);
    void DisplayImage();
    void RGBtoYUV();
    void YUVtoRGB();
    void CalculateAndDisplayHistograms();
    void ApplyHistogramEqualization();
    void ConvertToGrayscale();
    void GaussianFilter(int kernelSize, double sigma);
    void BlurFilter(int kernelSize);
    void ThresholdSegmentation(int threshold);
};
