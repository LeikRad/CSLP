#include "Image.h"
#include <iostream>
using namespace std;

int main()
{

    Image img("../../static/images/bike3.ppm");

    img.WriteImage("../../static/images/bike3Copy.ppm");

    // //testing YUV
    img.RGBtoYUV();
    img.WriteImage("../../static/images/bike3YUV.ppm");
    img.YUVtoRGB();
    img.WriteImage("../../static/images/bike3RGB.ppm");

    // //testing watermark
    Image watermark("../../static/images/baboon.ppm");
    img.WriteImageWaterMark(watermark);

    // //testing histogram
    img.CalculateAndDisplayHistograms();

    // testing histogram equalization
    img.ApplyHistogramEqualization();

    // testing grayscale
    img.ConvertToGrayscale();
    img.WriteImage("../../static/images/bike3Gray.ppm");

    return 0;
}