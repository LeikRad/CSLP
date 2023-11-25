#include <iostream>
#include "opencv2/opencv.hpp"
#include "BitStream.h"
#include "IntraDecoder.h"
#include "Predictors.h"
#include <bitset>

int main(int argc, char const *argv[])
{
    std::cout << "Enter the name of the file to read (absolute path): ";

    std::string input;

    std::cin >> input;

    // turn string to char

    char *input_char = new char[input.length() + 1];

    strcpy(input_char, input.c_str());

    BitStream bs(input_char, BitStream::READ);

    // decode format (yuv), width, height, frames, M, shift, predictor (0-7)

    std::bitset<8> binary_eight;
    std::string format = "";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            binary_eight[j] = bs.ReadBit();
        }
        format += char(binary_eight.to_ulong());
    }

    std::cout << "Format: " << format << std::endl;

    // decode n_frames, width, height

    int fps = 0;
    int num_frames = 0;
    int width = 0;
    int height = 0;

    for (int i = 0; i < 8; i++)
    {
        fps += bs.ReadBit() * pow(2, i);
    }

    for (int i = 0; i < 16; i++)
    {
        num_frames += bs.ReadBit() * pow(2, i);
    }

    for (int i = 0; i < 16; i++)
    {
        width += bs.ReadBit() * pow(2, i);
    }

    for (int i = 0; i < 16; i++)
    {
        height += bs.ReadBit() * pow(2, i);
    }

    std::cout << "fps: " << fps << std::endl;
    std::cout << "num_frames: " << num_frames << std::endl;
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;

    // decode M, shift, predictor (0-7)

    int M = 0;
    int shift = 0;
    int predictor = 0;

    for (int i = 0; i < 8; i++)
    {
        M += bs.ReadBit() * pow(2, i);
    }

    for (int i = 0; i < 8; i++)
    {
        shift += bs.ReadBit() * pow(2, i);
    }

    for (int i = 0; i < 8; i++)
    {
        predictor += bs.ReadBit() * pow(2, i);
    }

    std::cout << "M: " << M << std::endl;
    std::cout << "shift: " << shift << std::endl;
    std::cout << "predictor: " << predictor << std::endl;

    // get predictor function

    std::vector<std::function<int(int, int, int)>> func_vec = getPredictors();

    std::function<int(int, int, int)> func = func_vec[predictor];

    Golomb golomb(bs, M);

    IntraDecoder decoder(golomb, bs, shift);

    // array to store frames

    cv::Mat *frames = new cv::Mat[num_frames];

    // decode frames
    cv::VideoWriter writer = cv::VideoWriter("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height));
    for (int i = 0; i < num_frames; i++)
    {
        frames[i] = cv::Mat(height, width, CV_8UC3);
        std::cout << decoder.decode(frames[i], func) << std::endl;
        cv::cvtColor(frames[i], frames[i], cv::COLOR_YUV2BGR);

        writer.write(frames[i]);
    }

    // show video

    for (int i = 0; i < num_frames; i++)
    {
        cv::imshow("frame", frames[i]);
        cv::waitKey(30);
    }
}