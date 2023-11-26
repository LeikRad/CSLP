
#include "HybridEncoder.hpp"
#include "opencv2/opencv.hpp"
#include "Golomb.hpp"
#include <chrono>
#include <iostream>

HybridEncoder::HybridEncoder(string input_file, int predictor, int period, int block_range, int shift)
    : predictor(predictor), period(period), block_range(block_range), shift(shift)
{
    // read file header
    predictors = getPredictors();
    ifstream file(input_file, ios::binary);

    // Read the file header

    string file_header;

    getline(file, file_header);

    // split the file header into tokens

    istringstream iss(file_header);

    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};

    if (tokens.size() > 6)
    {
        if (tokens[6].compare("C444") == 0)
        {
            format = 0;
        }
        else if (tokens[6].compare("C422") == 0)
        {
            format = 1;
        }
    }
    else
    {
        format = 2;
    }

    // open the video file
    video = VideoCapture(input_file);

    video_width = video.get(CAP_PROP_FRAME_WIDTH);
    video_height = video.get(CAP_PROP_FRAME_HEIGHT);
    video_n_frames = video.get(CAP_PROP_FRAME_COUNT);
}

void HybridEncoder::encode(string output_file)
{
    Converter conv;
    GolombEncoder enc(output_file);

    IntraEncoder intra_enc(enc, shift);
    InterEncoder inter_enc(enc, block_size, block_range, shift);

    Mat curr_frame;
    Mat old_frame;

    enc.encode(format);
    enc.encode(predictor);
    enc.encode(block_range);
    enc.encode(shift);
    enc.encode(period);
    enc.encode(video_n_frames);

    int old_frame_cost = 0;
    int curr_frame_cost = 1;
    int count = 0;
    switch (format)
    {
    case 0:
    {
        while (true)
        {
            video >> curr_frame;
            if (curr_frame.empty())
            {
                break;
            };
            curr_frame = conv.rgb_to_yuv444(curr_frame);

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

                enc.encode(curr_frame.cols);
                enc.encode(curr_frame.rows);
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
            cout << "Encoded frame " << count++ << endl;
        }
        break;
    }
    case 1:
    {
        while (true)
        {
            video >> curr_frame;
            if (curr_frame.empty())
            {
                break;
            };
            curr_frame = conv.rgb_to_yuv422(curr_frame);

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

                enc.encode(curr_frame.cols);
                enc.encode(curr_frame.rows);
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
            cout << "Encoded frame " << count++ << endl;
        }
        break;
    }
    case 2:
    {
        while (true)
        {
            video >> curr_frame;
            if (curr_frame.empty())
            {
                break;
            };
            curr_frame = conv.rgb_to_yuv420(curr_frame);

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

                enc.encode(curr_frame.cols);
                enc.encode(curr_frame.rows);
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
            cout << "Encoded frame " << count++ << endl;
        }
        break;
    }
    }

    enc.finishEncoding();
}
