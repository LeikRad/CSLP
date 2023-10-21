#include "Image.h"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

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
Image::Image(const char *FileName)
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

void Image::WriteImage(const char *FileName)
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

void Image::RGBtoYUV()
{

    double Wr = 0.299;
    double Wb = 0.114;
    double Wg = 1 - Wr - Wb;
    double Umax = 0.436;
    double Vmax = 0.615;

    for (int i = 0; i < width * height; i++)
    {
        double r = pixels[i].r / 255.0;
        double g = pixels[i].g / 255.0;
        double b = pixels[i].b / 255.0;

        // Convert RGB to YUV
        double y = Wr * r + Wg * g + Wb * b;
        double u = Umax * ((b - y) / (1 - Wb));
        double v = Vmax * ((r - y) / (1 - Wr));

        pixels[i].r = y * 255;
        pixels[i].g = (u + 0.5) * 255;
        pixels[i].b = (v + 0.5) * 255;
    }
}

//////////////////////////////
// there is some noise here //
//////////////////////////////

void Image::YUVtoRGB()
{
    double Wr = 0.299;
    double Wb = 0.114;
    double Wg = 1 - Wr - Wb;
    double Umax = 0.436;
    double Vmax = 0.615;

    for (int i = 0; i < width * height; i++)
    {
        double y = pixels[i].r / 255.0;
        double u = (pixels[i].g / 255.0) - 0.5;
        double v = (pixels[i].b / 255.0) - 0.5;

        // Convert YUV to RGB
        double r = y + 1.13983 * v;
        double g = y - 0.39465 * u - 0.58060 * v;
        double b = y + 2.03211 * u;

        // Ensure that the values are within the valid range
        r = std::max(0.0, std::min(1.0, r));
        g = std::max(0.0, std::min(1.0, g));
        b = std::max(0.0, std::min(1.0, b));

        pixels[i].r = (u_char)(r * 255);
        pixels[i].g = (u_char)(g * 255);
        pixels[i].b = (u_char)(b * 255);
    }
}
void Image::CalculateAndDisplayHistograms()
{
    int histSize = 256;       // Number of bins
    float range[] = {0, 256}; // Range of pixel values
    const float *histRange = {range};

    Mat histR(histSize, 1, CV_32F, Scalar(0));
    Mat histG(histSize, 1, CV_32F, Scalar(0));
    Mat histB(histSize, 1, CV_32F, Scalar(0));

    // Calculate histograms for R, G, and B channels
    for (int i = 0; i < width * height; i++)
    {
        int r = pixels[i].r;
        int g = pixels[i].g;
        int b = pixels[i].b;

        histR.at<float>(r) += 1;
        histG.at<float>(g) += 1;
        histB.at<float>(b) += 1;
    }

    // Normalize the histograms to fit within the display area
    normalize(histR, histR, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, 1, NORM_MINMAX, -1, Mat());

    // Create a histogram plot and display it
    int plotWidth = 512;
    int plotHeight = 400;
    Mat plot(plotHeight, plotWidth, CV_8UC3, Scalar(255, 255, 255));

    int binWidth = cvRound((double)plotWidth / histSize);

    // Draw grid lines
    for (int i = 0; i < plotHeight; i += 20)
    {
        line(plot, Point(0, i), Point(plotWidth, i), Scalar(200, 200, 200), 1, LINE_AA);
    }
    for (int i = 0; i < plotWidth; i += 32)
    {
        line(plot, Point(i, 0), Point(i, plotHeight), Scalar(200, 200, 200), 1, LINE_AA);
    }

    // Draw x and y axes
    line(plot, Point(0, plotHeight - 1), Point(plotWidth - 1, plotHeight - 1), Scalar(0, 0, 0), 2, LINE_AA);
    line(plot, Point(0, plotHeight - 1), Point(0, 0), Scalar(0, 0, 0), 2, LINE_AA);

    // // Draw tick marks and labels on x axis
    // for (int i = 0; i < histSize; i += 32) {
    //     line(plot, Point(i * binWidth, plotHeight - 1), Point(i * binWidth, plotHeight - 5), Scalar(0, 0, 0), 2, LINE_AA);
    //     char buffer[16];
    //     sprintf(buffer, "%d", i);
    //     putText(plot, buffer, Point(i * binWidth, plotHeight - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    // }

    // Draw tick marks and labels on y axis
    for (int i = 0; i <= 10; i++)
    {
        line(plot, Point(0, plotHeight - 1 - i * plotHeight / 10), Point(5, plotHeight - 1 - i * plotHeight / 10), Scalar(0, 0, 0), 2, LINE_AA);
        char buffer[16];
        sprintf(buffer, "%d%%", i * 10);
        putText(plot, buffer, Point(10, plotHeight - 1 - i * plotHeight / 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    }

    for (int i = 1; i < histSize; i++)
    {
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histR.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histR.at<float>(i) * plotHeight)), Scalar(0, 0, 255), 2, LINE_AA);
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histG.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histG.at<float>(i) * plotHeight)), Scalar(0, 255, 0), 2, LINE_AA);
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histB.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histB.at<float>(i) * plotHeight)), Scalar(255, 0, 0), 2, LINE_AA);
    }

    // Add labels and title to the plot
    // putText(plot, "Pixel Value", Point(plotWidth / 2, plotHeight - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    // putText(plot, "Frequency", Point(10, plotHeight / 2), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    putText(plot, "Frequency (%) / Pixel Value", Point(plotWidth / 2, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);

    imshow("Histogram", plot);
    waitKey(0);
}

void Image::ApplyHistogramEqualization()
{
    Mat src(height, width, CV_8UC3, pixels);
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    // Split the HSV channels
    Mat channels[3];
    split(hsv, channels);

    // Calculate the histogram of the V channel
    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    Mat hist;
    calcHist(&channels[2], 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

    // Apply histogram equalization to the V channel
    equalizeHist(channels[2], channels[2]);

    // Merge the channels back into the HSV image
    Mat merged;
    merge(channels, 3, merged);

    // Convert the HSV image back to BGR
    Mat dst;
    cvtColor(merged, dst, COLOR_HSV2BGR);

    // Copy the modified pixels back to the image
    memcpy(pixels, dst.data, width * height * sizeof(Pixel));

    // Display the histogram
    int plotWidth = 512;
    int plotHeight = 256;
    Mat plot(plotHeight, plotWidth, CV_8UC3, Scalar(255, 255, 255));
    int binWidth = cvRound((double)plotWidth / histSize);
    normalize(hist, hist, 0, plotHeight, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(hist.at<float>(i - 1))),
             Point(binWidth * i, plotHeight - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), 2, LINE_AA);
    }
    imshow("Histogram", plot);
    waitKey(0);
}

void Image::ConvertToGrayscale()
{
    for (int i = 0; i < width * height; i++)
    {
        int gray = (int)(0.299 * pixels[i].r + 0.587 * pixels[i].g + 0.114 * pixels[i].b);
        pixels[i].r = gray;
        pixels[i].g = gray;
        pixels[i].b = gray;
    }
}