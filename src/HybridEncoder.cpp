
#include "HybridEncoder.hpp"
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"
#include "YUVReader.hpp"
#include <chrono>
#include <iostream>

HybridEncoder::HybridEncoder(string input_file, int predictor, int period, int block_range, int shift)
    : predictor(predictor), period(period), block_range(block_range), shift(shift), video(input_file)
{
    // read file header
    predictors = getPredictors();

    this->video_n_frames = video.get_n_frames();

}

void HybridEncoder::encode(string output_file)
{
    Converter conv;
    GolombEncoder enc(output_file);

    IntraEncoder intra_enc(enc, shift);
    InterEncoder inter_enc(enc, block_size, block_range, shift);

    Mat curr_frame;
    Mat old_frame;

    enc.encode(predictor);
    enc.encode(block_range);
    enc.encode(shift);
    enc.encode(period);
    enc.encode(video_n_frames);

    int old_frame_cost = 0;
    int curr_frame_cost = 1;
    int count = 0;
    for (char c : video.get_file_header())
    {
        int a = static_cast<int>(c);
        enc.encode(a);
    }
    // encode newline
    enc.encode(10);

    while (video.nextFrame_exists())
        {
            curr_frame = video.get_nextFrame();
            if (curr_frame.empty())
            {
                break;
            };

            if (count == 0)
            {
                int a = curr_frame.cols;
                int b = curr_frame.rows;
                if (a != b)
                {
                    int gcd = -1;
                    while (b != 0)
                    {
                        a %= b;
                        if (a == 0)
                            gcd = b;
                        break;
                        b %= a;
                    }
                    if (gcd == -1)
                        gcd = a;
                    if (gcd == a || gcd == b)
                        gcd = 16;
                    this->block_size = gcd;
                    inter_enc.setBlockSize(gcd);
                    enc.encode(gcd);
                }
                else
                {
                    this->block_size = 16;
                    inter_enc.setBlockSize(16);
                    enc.encode(16);
                }
            }

            if (curr_frame_cost > old_frame_cost || count % period == 0)
            {
                curr_frame.copyTo(old_frame);
                curr_frame_cost = intra_enc.encode(curr_frame, predictors[predictor]);
                old_frame_cost = curr_frame_cost;
            }
            else
            {
                curr_frame_cost = inter_enc.encode(old_frame, curr_frame);
            }
            count++;
        }

    enc.finishEncoding();
}
