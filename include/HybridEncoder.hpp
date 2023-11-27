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
    int format;                                      ///< Format in which the video will be stored
    int video_width;                                 ///< Width of the video
    int video_height;                                ///< Height of the video
    int video_n_frames;                              ///< Number of video frames
    int predictor;                                   ///< Predictor used in intra-frame encoding
    int period;                                      ///< Period used to decide when to code intra
    int block_size;                                  ///< Block size used in inter-frame encoding
    int block_range;                                 ///< Block range used in inter-frame encoding
    int shift;                                       ///< Quantization value used for lossy coding
    vector<function<int(int, int, int)>> predictors; ///< List of predictors

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