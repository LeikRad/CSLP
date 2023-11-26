/**
 * @file HybridEncoder.hpp
 * @brief Hybrid Encoder for video compression
 *
 * This file contains the declaration of the HybridEncoder class used for video compression.
 */

#ifndef HYBRID_ENCODER_HPP
#define HYBRID_ENCODER_HPP

#include <opencv2/opencv.hpp>
#include "Golomb.hpp"
#include "Predictors.hpp"
#include "InterEncoder.hpp"
#include "IntraEncoder.hpp"
#include "Converter.hpp"
#include <iterator>
#include <vector>

using namespace cv;
using namespace std;

/**
 * @class HybridEncoder
 * @brief Class for hybrid video encoding
 *
 * This class uses both inter-frame and intra-frame encoding for video compression.
 */
class HybridEncoder
{
private:
    VideoCapture video;                              ///< Video to be encoded
    int format;                                      ///< Format of the video
    int width;                                       ///< Width of the video
    int height;                                      ///< Height of the video
    int n_frames;                                    ///< Number of frames in the video
    int predictor;                                   ///< Predictor used for encoding
    int period;                                      ///< Period for intra-frame encoding
    vector<function<int(int, int, int)>> predictors; ///< List of predictors
    int block_size;                                  ///< Size of the blocks for inter-frame encoding
    int block_range;                                 ///< Range of the blocks for inter-frame encoding
    int shift;                                       ///< Shift for inter-frame encoding

public:
    /**
     * @brief Construct a new Hybrid Encoder object
     *
     * @param input_file Path to the input video file
     * @param predictor Predictor to be used for encoding
     * @param period Period for intra-frame encoding
     * @param block_range Range of the blocks for inter-frame encoding
     * @param shift Shift for inter-frame encoding
     */
    HybridEncoder(string input_file, int predictor = 7, int period = 20, int block_range = 4, int shift = 0);

    /**
     * @brief Encode the video
     *
     * @param output_file Path to the output file
     */
    void encode(string output_file);
};

#endif // HYBRID_ENCODER_HPP