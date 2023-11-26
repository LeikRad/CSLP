/**
 * @file InterEncoder.hpp
 * @brief Inter Encoder for video compression
 *
 * This file contains the declaration of the InterEncoder class used for video compression.
 */

#ifndef INTERENCODER_H
#define INTERENCODER_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"

using namespace cv;
using namespace std;

/**
 * @class InterEncoder
 * @brief Class for inter-frame encoding
 *
 * This class is used for inter-frame encoding in video compression.
 */
class InterEncoder
{
private:
    int block_size;        ///< Size of the blocks for encoding
    int block_range;       ///< Range of the blocks for encoding
    int shift;             ///< Shift for encoding
    GolombEncoder &golomb; ///< Golomb encoder for encoding

public:
    /**
     * @brief Construct a new Inter Encoder object
     *
     * @param golomb Golomb encoder for encoding
     * @param block_size Size of the blocks for encoding
     * @param block_range Range of the blocks for encoding
     * @param shift Shift for encoding
     */
    InterEncoder(GolombEncoder &golomb, int block_size, int block_range, int shift = 0);

    /**
     * @brief Default constructor
     */
    InterEncoder() = default;

    /**
     * @brief Get the block size
     *
     * @return Block size
     */
    int getBlockSize() const;

    /**
     * @brief Get the block range
     *
     * @return Block range
     */
    int getBlockRange() const;

    /**
     * @brief Set the block size
     *
     * @param block_size New block size
     */
    void setBlockSize(int block_size);

    /**
     * @brief Calculate the cost of encoding a block
     *
     * @param block Block to be encoded
     * @return Cost of encoding the block
     */
    float cost(Mat block);

    /**
     * @brief Encode a frame
     *
     * @param old_frame Previous frame
     * @param new_frame Current frame
     * @return Result of the encoding
     */
    int encode(Mat &old_frame, Mat &new_frame);
};
#endif // INTERENCODER_H