#ifndef INTRADECODER_H
#define INTRADECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;
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
    GolombDecoder &golomb;

public:
    IntraDecoder(GolombDecoder &golomb, int shift = 0);

    ~IntraDecoder();

    int decode(Mat &frame, function<int(int, int, int)> predictor);
};
#endif // INTRADECODER_H