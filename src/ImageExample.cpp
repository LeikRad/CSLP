#include "image.h"
#include <iostream>
using namespace std;

int main()
{

    Image img("../../images/bike3.ppm");

    // file = fopen("../images/airplane.ppm", "rb");

    // if (!file)
    // {
    //     std::cerr << "Failed to open the file." << std::endl;
    //     return 1;
    // }
    // Image img2(file);
    // fclose(file);
    img.WriteImage("../../images/bike3Copy.ppm");

    // //testing YUV
    // img.RGBtoYUV();
    // img.WriteImage("../../images/bike3YUV.ppm");
    // img.YUVtoRGB();
    // img.WriteImage("../../images/bike3RGB.ppm");

    // //testing histogram
    // img.CalculateAndDisplayHistograms();

    // testing histogram equalization
    img.ApplyHistogramEqualization();


    //testing grayscale
    img.ConvertToGrayscale();
    img.WriteImage("../../images/bike3Gray.ppm");

    return 0;
}