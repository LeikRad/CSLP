#include "BitStream.h"
#include "IntraDecoder.h"

IntraDecoder::IntraDecoder(Golomb &golomb, BitStream &bs, int shift) : golomb(golomb), bs(bs), shift(shift)
{
}

IntraDecoder::~IntraDecoder()
{
}

int IntraDecoder::decode(cv::Mat &frame, std::function<int(int, int, int)> predictor)
{
    if (!bs.can_read())
    {
        std::cerr << "Error: BitStream is not ready to read or write" << std::endl;
        return -1;
    }

    int a, b, c, err, frame_cost = 0, n_ch = frame.channels();

    if (n_ch == 3)
    {
        hconcat(cv::Mat::zeros(frame.rows, 1, CV_8UC3), frame, frame);
        vconcat(cv::Mat::zeros(1, frame.cols, CV_8UC3), frame, frame);
    }
    else if (n_ch == 1)
    {
        hconcat(cv::Mat::zeros(frame.rows, 1, CV_8UC1), frame, frame);
        vconcat(cv::Mat::zeros(1, frame.cols, CV_8UC1), frame, frame);
    }
    int optimalM = golomb.Decode();
    std::cout << "optimalM: " << optimalM << std::endl;

    if (optimalM != 0)
        golomb.setM(optimalM);

    for (int i = 1; i < frame.rows; i++)
    {
        for (int n = 1; n < frame.cols; n++)
        {
            for (int ch = 0; ch < n_ch; ch++)
            {
                a = frame.ptr<uchar>(i, n - 1)[ch];
                b = frame.ptr<uchar>(i - 1, n)[ch];
                c = frame.ptr<uchar>(i - 1, n - 1)[ch];

                err = golomb.Decode();
                frame_cost += abs(err);

                if (err < 0)
                {
                    err = -1 * (abs(err) << shift);
                }
                else
                {
                    err <<= shift;
                }
                frame.ptr<uchar>(i, n)[ch] = (unsigned char)predictor(a, b, c) + err;
            }
        }
    }
    frame = frame(cv::Rect(1, 1, frame.cols - 1, frame.rows - 1));
    return frame_cost / (frame.rows * frame.cols * frame.channels());
}
