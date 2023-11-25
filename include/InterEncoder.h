#ifndef INTERENCODER_H
#define INTERENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

/**
 *
 * @brief    Class for inter encoder.
 *
 *
 */

class InterEncoder
{
private:
    int block_size;
    int block_range;
    int shift;
    Golomb &golomb;
    BitStream &bs;

public:
    InterEncoder(Golomb &golomb, BitStream &bs, int block_size, int block_range, int shift = 0);
    InterEncoder() = default;

    int getBlockSize() const;

    int getBlockRange() const;

    void setBlockSize(int block_size);

    float cost(Mat block);

    int encode(cv::Mat &old_frame, cv::Mat &new_frame);
};
#endif // INTERENCODER_H