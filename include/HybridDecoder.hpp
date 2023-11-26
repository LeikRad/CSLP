/**
 * @file HybridDecoder.hpp
 * @brief Hybrid Decoder for video decompression
 *
 * This file contains the declaration of the HybridDecoder class used for video decompression.
 */

#ifndef HYBRID_DECODER_HPP
#define HYBRID_DECODER_HPP

#include "Converter.hpp"
#include "Golomb.hpp"
#include "InterDecoder.hpp"
#include "IntraDecoder.hpp"
#include <vector>
#include "Predictors.hpp"
using namespace cv;
using namespace std;

/**
 * @class HybridDecoder
 * @brief Class for hybrid video decoding
 *
 * This class uses both inter-frame and intra-frame decoding for video decompression.
 */
class HybridDecoder
{
private:
    string input_file;                               ///< Path to the input file
    vector<function<int(int, int, int)>> predictors; ///< List of predictors

public:
    /**
     * @brief Construct a new Hybrid Decoder object
     *
     * @param input_file Path to the input video file
     */
    HybridDecoder(string input_file);

    /**
     * @brief Decode the video
     *
     * @param output_file Path to the output file
     */
    void decode(string output_file);
};

#endif // HYBRID_DECODER_HPP