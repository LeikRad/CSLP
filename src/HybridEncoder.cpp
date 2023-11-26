
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

    width = video.get(CAP_PROP_FRAME_WIDTH);
    height = video.get(CAP_PROP_FRAME_HEIGHT);
    n_frames = video.get(CAP_PROP_FRAME_COUNT);
}

void HybridEncoder::encode(string output_file)
{
    Converter converter;
    GolombEncoder enc(output_file);

    IntraEncoder intra_enc(enc, shift);
    InterEncoder inter_enc(enc, block_size, block_range, shift);

    Mat new_frame;
    Mat old_frame;

    cout << "Encoding file " << output_file << endl;
    cout << "format: " << format << endl;
    cout << "predictor: " << predictor << endl;
    cout << "shift: " << shift << endl;
    cout << "block_range: " << block_range << endl;
    cout << "period: " << period << endl;
    cout << "n_frames: " << n_frames << endl;

    enc.encode(format);
    enc.encode(predictor);
    enc.encode(shift);
    enc.encode(block_range);
    enc.encode(period);
    enc.encode(n_frames);

    int old_cost = 0, new_cost = 1, frame_n = 0;

    switch (format)
    {
    case 0:
    {
        while (true)
        {
            video >> new_frame;
            if (new_frame.empty())
            {
                break;
            };
            new_frame = converter.rgb_to_yuv444(new_frame);

            if (frame_n == 0)
            {
                int a = new_frame.cols;
                int b = new_frame.rows;
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

                enc.encode(new_frame.cols);
                enc.encode(new_frame.rows);
            }

            if (new_cost > old_cost || frame_n % 20 == 0)
            {
                new_frame.copyTo(old_frame);
                new_cost = intra_enc.encode(new_frame, predictors[predictor]);
                old_cost = new_cost;
            }
            else
            {
                new_cost = inter_enc.encode(old_frame, new_frame);
            }
            break;
        }
        break;
    }
    case 1:
    {
        while (true)
        {
            video >> new_frame;
            if (new_frame.empty())
            {
                break;
            };
            new_frame = converter.rgb_to_yuv422(new_frame);

            if (frame_n == 0)
            {
                int a = new_frame.cols;
                int b = new_frame.rows;
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

                enc.encode(new_frame.cols);
                enc.encode(new_frame.rows);
            }

            if (new_cost > old_cost || frame_n % 20 == 0)
            {
                new_frame.copyTo(old_frame);
                new_cost = intra_enc.encode(new_frame, predictors[predictor]);
                old_cost = new_cost;
            }
            else
            {
                new_cost = inter_enc.encode(old_frame, new_frame);
            }
            cout << "Encoded frame " << frame_n++ << endl;
        }
        break;
    }
    case 2:
    {
        while (true)
        {
            video >> new_frame;
            if (new_frame.empty())
            {
                break;
            };
            new_frame = converter.rgb_to_yuv420(new_frame);
            cout << "converted frame " << frame_n << endl;

            if (frame_n == 0)
            {
                int a = new_frame.cols;
                int b = new_frame.rows;
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

                enc.encode(new_frame.cols);
                enc.encode(new_frame.rows);
            }

            if (new_cost > old_cost || frame_n % 20 == 0)
            {
                new_frame.copyTo(old_frame);
                new_cost = intra_enc.encode(new_frame, predictors[predictor]);
                old_cost = new_cost;
            }
            else
            {
                new_cost = inter_enc.encode(old_frame, new_frame);
            }
            cout << "Encoded frame " << frame_n++ << endl;
        }
        break;
    }
    }

    enc.finishEncoding();
}
