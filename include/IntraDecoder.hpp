/**
 * @file IntraDecoder.hpp
 * @brief Intra Decoder for video decompression
 *
 * This file contains the declaration of the IntraDecoder class used for video decompression.
 */

#ifndef INTRADECODER_H
#define INTRADECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;

/**
 * @class IntraDecoder
 * @brief Class for intra-frame decoding
 *
 * This class is used for intra-frame decoding in video decompression.
 */
class IntraDecoder
{
private:
    int shift;             ///< Shift for decoding
    GolombDecoder &golomb; ///< Golomb decoder for decoding

public:
    /**
     * @brief Construct a new Intra Decoder object
     *
     * @param golomb Golomb decoder for decoding
     * @param shift Shift for decoding
     */
    IntraDecoder(GolombDecoder &golomb, int shift = 0);

    /**
     * @brief Destroy the Intra Decoder object
     */
    ~IntraDecoder();

    /**
     * @brief Decode a frame
     *
     * @param frame Frame to be decoded
     * @param predictor Predictor to be used for decoding
     * @return Result of the decoding
     */
    int decode(Mat &frame, function<int(int, int, int)> predictor);
};
#endif // INTRADECODER_H