#ifndef INTRADECODER_H
#define INTRADECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

/**
 *
 * @brief    Class for intra encoder.
 *
 *
 */

class IntraDecoder
{
private:
    int shift;
    Golomb &golomb;
    BitStream &bs;

public:
    IntraDecoder(Golomb &golomb, BitStream &bs, int shift = 0);

    ~IntraDecoder();

    int decode(cv::Mat &frame, std::function<int(int, int, int)> predictor);
};
#endif // INTRADECODER_H