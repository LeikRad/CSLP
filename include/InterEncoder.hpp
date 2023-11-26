#ifndef INTERENCODER_H
#define INTERENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;
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
    GolombEncoder &golomb;

public:
    InterEncoder(GolombEncoder &golomb, int block_size, int block_range, int shift = 0);

    InterEncoder() = default;

    int getBlockSize() const;

    int getBlockRange() const;

    void setBlockSize(int block_size);

    float cost(Mat block);

    int encode(Mat &old_frame, Mat &new_frame);
};
#endif // INTERENCODER_H