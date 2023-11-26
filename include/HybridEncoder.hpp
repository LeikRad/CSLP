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
class HybridEncoder
{
private:
    /** Video to be encoded. */
    VideoCapture video;

    /** Format in which the video will be stored. */
    int format;

    /** Width of the video. */
    int video_width;

    /** Height of the video. */
    int video_height;

    /** Number of video frames. */
    int video_n_frames;

    /** Predictor used in intra-frame encoding. */
    int predictor;

    /** Period used to dedcide when to code intra */
    int period;

    /** Block size used in inter-frame encoding. */
    int block_size;

    /** Block range used in inter-frame encoding. */
    int block_range;

    /** Quantization value used for lossy coding. */
    int shift;
    vector<function<int(int, int, int)>> predictors;

public:
    HybridEncoder(string input_file, int predictor = 7, int period = 20, int block_range = 5, int shift = 0);
    void encode(string output_file);
};

#endif // HYBRID_ENCODER_HPP