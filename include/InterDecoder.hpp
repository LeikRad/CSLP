/**
 * @file InterDecoder.hpp
 * @brief Inter Decoder for video decompression
 *
 * This file contains the declaration of the InterDecoder class used for video decompression.
 */

#ifndef INTERDECODER_H
#define INTERDECODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;

/**
 * @class InterDecoder
 * @brief Class for inter-frame decoding
 *
 * This class is used for inter-frame decoding in video decompression.
 */
class InterDecoder
{
private:
    int block_size;        ///< Size of the blocks for decoding
    int block_range;       ///< Range of the blocks for decoding
    int shift;             ///< Shift for decoding
    GolombDecoder &golomb; ///< Golomb decoder for decoding

public:
    /**
     * @brief Construct a new Inter Decoder object
     *
     * @param golomb Golomb decoder for decoding
     * @param block_size Size of the blocks for decoding
     * @param block_range Range of the blocks for decoding
     * @param shift Shift for decoding
     */
    InterDecoder(GolombDecoder &golomb, int block_size, int block_range, int shift = 0);

    /**
     * @brief Default constructor
     */
    InterDecoder() = default;

    /**
     * @brief Decode a frame
     *
     * @param old_frame Previous frame
     * @param new_frame Current frame
     * @return Result of the decoding
     */
    int decode(Mat &old_frame, Mat &new_frame);
};
#endif // INTERDECODER_H