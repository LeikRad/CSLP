#include "Image.h"
#include <iostream>
using namespace std;

int main()
{

    Image img("../../static/images/bike3.ppm");

    img.WriteImage("../../static/images/bike3Copy.ppm");

    // //testing water mark
    img.WriteImageWaterMark("../../static/images/bike3WaterMark.ppm");

    // //testing YUV
    img.RGBtoYUV();
    img.WriteImage("../../static/images/bike3YUV.ppm");
    img.YUVtoRGB();
    img.WriteImage("../../static/images/bike3RGB.ppm");

    // //testing histogram
    // img.CalculateAndDisplayHistograms();

    

    // testing grayscale
    Image img4("../../static/images/bike3.ppm");
    img4.ConvertToGrayscale();
    img4.WriteImage("../../static/images/bike3Gray.ppm");

    // testing histogram equalization
    // img4.ApplyHistogramEqualization();


    Image img2("../../static/images/bike3.ppm");

    // testing gaussian filter
    img2.GaussianFilter(5, 5.0);
    img2.WriteImage("../../static/images/bike3Gaussian.ppm");

    Image img3("../../static/images/bike3.ppm");

    // testing blur filter
    img3.BlurFilter(5);
    img3.WriteImage("../../static/images/bike3Blur.ppm");

    // testing threshold segmentation
    
    img4.ThresholdSegmentation(100);
    img4.WriteImage("../../static/images/bike3Threshold.ppm");

    return 0;
}