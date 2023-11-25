#ifndef INTRAENCODER_H
#define INTRAENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.h"

/**
 *
 * @brief    Class for intra encoder.
 *
 *  Error values are calculated by the difference between the original and the predicted values and then encoded by Golomb.
 */

class IntraEncoder
{
private:
    int shift;
    Golomb &golomb;
    BitStream &bs;

public:
    IntraEncoder(Golomb &golomb, BitStream &bs, int shift = 0);

    ~IntraEncoder();

    int encode(cv::Mat &frame, std::function<int(int, int, int)> predictor);
};
#endif // INTRAENCODER_H