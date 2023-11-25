#ifndef INTERDECODER_H
#define INTERDECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

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
    Golomb &golomb;
    BitStream &bs;

public:
    InterDecoder(Golomb &golomb, BitStream &bs, int block_size, int block_range, int shift = 0);
    InterDecoder() = default;
    int decode(cv::Mat &old_frame, cv::Mat &new_frame);
}
#endif // INTERDECODER_H