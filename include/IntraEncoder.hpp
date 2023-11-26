#ifndef INTRAENCODER_H
#define INTRAENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;

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
    GolombEncoder &golomb;

public:
    IntraEncoder(GolombEncoder &golomb, int shift = 0);

    ~IntraEncoder();

    int encode(Mat &frame, function<int(int, int, int)> predictor);
};
#endif // INTRAENCODER_H