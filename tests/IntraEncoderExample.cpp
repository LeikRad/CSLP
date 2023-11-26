#include <iostream>
#include "opencv2/opencv.hpp"
#include "BitStream.hpp"
#include "IntraEncoder.hpp"
#include "Predictors.hpp"
#include <chrono>
#include <iomanip>
#include <iterator>
#include "Converter.hpp"

int main(int argc, char const *argv[])
{
    Converter conv;
    vector<function<int(int, int, int)>> predictors = getPredictors();
    cout
        << "Enter the name of the file to save to (absolute path): ";

    string output;

    cin >> output;

    cout << "Enter the name of the file to read from (absolute path): ";

    string input;

    cin >> input;

    // read file header

    ifstream file(input, ios::binary);

    // Read the file header

    string file_header;

    getline(file, file_header);

    // split the file header into tokens

    istringstream iss(file_header);

    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};

    // write the tokens to the console
    int format;
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

    // print the format

    cout << "Format: " << format << endl;

    VideoCapture cap(input);

    if (!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    int shift = 0;
    int predictor = 7;
    GolombEncoder encoder(output);
    IntraEncoder intra_encoder(encoder, shift);
    Converter converter;

    Mat frame;
    Mat yuv;
    encoder.encode(format);
    encoder.encode(predictor);
    encoder.encode(shift);
    encoder.encode(cap.get(CAP_PROP_FRAME_COUNT));

    int n_frame = 0;

    switch (format)
    {
    case 0:
    {
        while (true)
        {
            cap >> frame;
            if (frame.empty())
            {
                break;
            };
            frame = conv.rgb_to_yuv444(frame);

            if (n_frame == 0)
            {
                encoder.encode(frame.cols);
                encoder.encode(frame.rows);
            }

            intra_encoder.encode(frame, predictors[predictor]);

            cout << "Encoded frame " << n_frame++ << endl;
        }
        break;
    }
    case 1:
    {
        while (true)
        {
            cap >> frame;
            if (frame.empty())
            {
                break;
            };
            frame = conv.rgb_to_yuv422(frame);

            if (n_frame == 0)
            {
                encoder.encode(frame.cols);
                encoder.encode(frame.rows);
            }

            intra_encoder.encode(frame, predictors[predictor]);

            cout << "Encoded frame " << n_frame++ << endl;
        }
        break;
    }
    case 2:
    {
        while (true)
        {
            cap >> frame;
            if (frame.empty())
            {
                break;
            };
            frame = conv.rgb_to_yuv420(frame);

            if (n_frame == 0)
            {
                encoder.encode(frame.cols);
                encoder.encode(frame.rows);
            }

            intra_encoder.encode(frame, predictors[predictor]);

            cout << "Encoded frame " << n_frame++ << endl;
        }
        break;
    }
    }

    encoder.finishEncoding();
    return 0;
}