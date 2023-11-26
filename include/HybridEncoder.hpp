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
    VideoCapture video;
    int format;
    int width;
    int height;
    int n_frames;
    int predictor;
    int period;
    vector<function<int(int, int, int)>> predictors;
    int block_size;
    int block_range;
    int shift;

public:
    HybridEncoder(string input_file, int predictor = 7, int period = 20, int block_range = 4, int shift = 0);
    void encode(string output_file);
};
#endif // HYBRID_ENCODER_HPP