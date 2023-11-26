/**
 * @file IntraEncoder.hpp
 * @brief Intra Encoder for video compression
 *
 * This file contains the declaration of the IntraEncoder class used for video compression.
 */

#ifndef INTRAENCODER_H
#define INTRAENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;

/**
 * @class IntraEncoder
 * @brief Class for intra-frame encoding
 *
 * This class is used for intra-frame encoding in video compression.
 * Error values are calculated by the difference between the original and the predicted values and then encoded by Golomb.
 */
class IntraEncoder
{
private:
    int shift;             ///< Shift for encoding
    GolombEncoder &golomb; ///< Golomb encoder for encoding

public:
    /**
     * @brief Construct a new Intra Encoder object
     *
     * @param golomb Golomb encoder for encoding
     * @param shift Shift for encoding
     */
    IntraEncoder(GolombEncoder &golomb, int shift = 0);

    /**
     * @brief Destroy the Intra Encoder object
     */
    ~IntraEncoder();

    /**
     * @brief Encode a frame
     *
     * @param frame Frame to be encoded
     * @param predictor Predictor to be used for encoding
     * @return Result of the encoding
     */
    int encode(Mat &frame, function<int(int, int, int)> predictor);
};
#endif // INTRAENCODER_H