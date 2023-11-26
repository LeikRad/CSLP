#ifndef INTERDECODER_H
#define INTERDECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;
/**
 *
 * @brief    Class for inter decoder.
 *
 *
 */
class InterDecoder
{
private:
    int block_size;
    int block_range;
    int shift;
    GolombDecoder &golomb;

public:
    InterDecoder(GolombDecoder &golomb, int block_size, int block_range, int shift = 0);
    InterDecoder() = default;
    int decode(Mat &old_frame, Mat &new_frame);
};
#endif // INTERDECODER_H