/**
 * @file Image.hpp
 * @brief Header file for the Image class
 *
 * This header file defines the Image class, which represents a digital image.
 * The class provides methods for reading and writing image files, applying various image processing operations,
 * and displaying the image on the screen.
 */

#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>

using namespace std;

/**
 * @struct Pixel
 * @brief A struct representing a pixel in an image
 *
 * The `Pixel` struct represents a pixel in an image, with separate red, green, and blue color channels.
 */
struct Pixel
{
    u_char r; /**< The red color channel of the pixel */
    u_char g; /**< The green color channel of the pixel */
    u_char b; /**< The blue color channel of the pixel */
};

/**
 * @brief A class representing a digital image
 *
 * The `Image` class represents a digital image, with methods for reading and writing image files,
 * applying various image processing operations, and displaying the image on the screen.
 */
class Image
{
private:
    int width;     /**< The width of the image in pixels */
    int height;    /**< The height of the image in pixels */
    int maxVal;    /**< The maximum pixel value of the image */
    Pixel *pixels; /**< A pointer to the pixel data of the image */

public:
    /**
     * @brief Construct a new Image object from a file
     *
     * The constructor reads an image file from disk and creates a new `Image` object from it.
     *
     * @param FileName The name of the image file to read
     */
    Image(const char *FileName);

    /**
     * @brief Destroy the Image object and free the memory used by the pixel data
     *
     * The destructor frees the memory used by the pixel data of the image.
     */
    ~Image();

    /**
     * @brief Get the metadata of the image
     *
     * The `Metadata` method returns an array of integers containing the width, height, and maximum pixel value of the image.
     *
     * @return An array of integers containing the width, height, and maximum pixel value of the image
     */
    int *Metadata();

    /**
     * @brief Get a pointer to the pixel data of the image
     *
     * The `PixelData` method returns a pointer to the pixel data of the image.
     *
     * @return A pointer to the pixel data of the image
     */
    Pixel *PixelData();

    /**
     * @brief Write the image to a file
     *
     * The `WriteImage` method writes the image to a file with the specified name.
     *
     * @param FileName The name of the file to write the image to
     */
    void WriteImage(const char *FileName);

    /**
     * @brief Write a watermark image to the image
     *
     * The `WriteImageWaterMark` method writes a watermark image to the image.
     *
     * @param watermarkImage The watermark image to write to the image
     */
    void WriteImageWaterMark(Image &watermarkImage);

    /**
     * @brief Display the image on the screen
     *
     * The `DisplayImage` method displays the image on the screen using OpenCV.
     */
    void DisplayImage();

    /**
     * @brief Convert the image from RGB to YUV color space
     *
     * The `RGBtoYUV` method converts the image from RGB color space to YUV color space.
     */
    void RGBtoYUV();

    /**
     * @brief Convert the image from YUV to RGB color space
     *
     * The `YUVtoRGB` method converts the image from YUV color space to RGB color space.
     */
    void YUVtoRGB();

    /**
     * @brief Calculate and display the histograms of the image
     *
     * The `CalculateAndDisplayHistograms` method calculates and displays the histograms of the image using OpenCV.
     */
    void CalculateAndDisplayHistograms();

    /**
     * @brief Apply histogram equalization to the V channel of the image in the HSV color space
     *
     * The `ApplyHistogramEqualization` method applies histogram equalization to the V channel of the image in the HSV color space.
     */
    void ApplyHistogramEqualization();

    /**
     * @brief Convert the image to grayscale
     *
     * The `ConvertToGrayscale` method converts the image to grayscale by taking the average of its red, green, and blue values.
     */
    void ConvertToGrayscale();

    /**
     * @brief Apply a Gaussian filter to the image
     *
     * The `GaussianFilter` method applies a Gaussian filter to the image using the specified kernel size and sigma value.
     */
    void GaussianFilter(int kernelSize, double sigma);

    /**
     * @brief Apply a blur filter to the image
     *
     * The `BlurFilter` method applies a blur filter to the image using the specified kernel size.
     */
    void BlurFilter(int kernelSize);

    /**
     * @brief Apply threshold segmentation to the image
     *
     * The `ThresholdSegmentation` method applies threshold segmentation to the image using the specified threshold value.
     */
    void ThresholdSegmentation(int threshold);
};
#endif // IMAGE_H