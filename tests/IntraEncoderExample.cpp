#include <iostream>
#include "opencv2/opencv.hpp"
#include "BitStream.h"
#include "IntraEncoder.h"
#include "Predictors.h"
#include <bitset>
#include <chrono>
#include <iomanip>

int main(int argc, char const *argv[])
{
    std::cout << "Enter the name of the file to save to (absolute path): ";

    std::string input;

    std::cin >> input;

    // convert string to char

    char input_char[input.length() + 1];

    strcpy(input_char, input.c_str());

    BitStream bs(input_char, BitStream::WRITE);

    std::cout << "Enter the name of the file to read from (absolute path): ";

    std::cin >> input;

    cv::VideoCapture cap(input);

    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    char format[3] = {'y', 'u', 'v'};

    std::bitset<8> binary_eight;
    std::bitset<16> binary_sixteen;
    for (int i = 0; i < 3; i++)
    { // convert char to bitset
        binary_eight = std::bitset<8>(format[i]);
        for (int j = 0; j < 8; j++)
        {
            bs.WriteBit(binary_eight[j]);
        }
    }

    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // get number of frames

    int num_frames = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int fps = cap.get(cv::CAP_PROP_FPS);
    std::cout
        << "num_frames: " << num_frames << std::endl;

    std::cout
        << "fps: " << fps << std::endl;

    // encode fps, num, frames, width and height
    // convert width to bitset

    binary_eight = std::bitset<8>(fps);
    std::cout << "fps: " << binary_eight << std::endl;

    for (int i = 0; i < 8; i++)
    {
        bs.WriteBit(binary_eight[i]);
    }

    binary_sixteen = std::bitset<16>(num_frames);
    std::cout << "num_frames: " << binary_sixteen << std::endl;

    for (int i = 0; i < 16; i++)
    {
        bs.WriteBit(binary_sixteen[i]);
    }

    binary_sixteen = std::bitset<16>(width);
    std::cout << "width: " << binary_sixteen << std::endl;

    for (int i = 0; i < 16; i++)
    {
        bs.WriteBit(binary_sixteen[i]);
    }

    // convert height to bitset

    binary_sixteen = std::bitset<16>(height);
    std::cout << "height: " << binary_sixteen << std::endl;

    for (int i = 0; i < 16; i++)
    {
        bs.WriteBit(binary_sixteen[i]);
    }

    // convert m to bitset

    int m = 8;
    binary_eight = std::bitset<8>(m);

    std::cout << "m: " << binary_eight << std::endl;

    for (int i = 0; i < 8; i++)
    {
        bs.WriteBit(binary_eight[i]);
    }

    // write shift to bitstream

    int shift = 0;
    binary_eight = std::bitset<8>(shift);
    std::cout << "shift: " << binary_eight << std::endl;

    for (int i = 0; i < 8; i++)
    {
        bs.WriteBit(binary_eight[i]);
    }

    // get predictor list

    std::vector<std::function<int(int, int, int)>> predictors = getPredictors();

    int predictor = 7;

    // write predictor used to bitstream

    binary_eight = std::bitset<8>(predictor);

    for (int i = 0; i < 8; i++)
    {
        bs.WriteBit(binary_eight[i]);
    }

    // go through every frame and encode it

    cv::Mat frame;
    cv::Mat frame_yuv;

    Golomb golomb(bs, m);

    IntraEncoder intra_encoder(golomb, bs, shift);

    double percentage = 0.0;
    double average_time = 0.0;
    int count = 0;
    while (1)
    {
        percentage = (cap.get(cv::CAP_PROP_POS_FRAMES) / num_frames) * 100;
        std::cout << "Frame %: " << std::fixed << std::setprecision(1) << percentage << "\% ETA ~ " << std::fixed << std::setprecision(1) << average_time * (100 - percentage) << "s" << std::endl;
        cap >> frame;

        if (frame.empty())
        {
            break;
        }

        cv::cvtColor(frame, frame_yuv, cv::COLOR_BGR2YUV);
        auto start = std::chrono::high_resolution_clock::now(); // start timer

        intra_encoder.encode(frame_yuv, predictors[predictor]);
        auto end = std::chrono::high_resolution_clock::now(); // end timer
        double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        average_time *= count;
        average_time += time_taken;
        average_time /= ++count;
        // move to start of previous line
        std::cout << "\033[F" << std::flush;
        // clear line
        std::cout << "\033[K" << std::flush;
    }
}