#include "BitStream.hpp"
#include "IntraDecoder.hpp"

IntraDecoder::IntraDecoder(GolombDecoder &golomb, int shift) : golomb(golomb), shift(shift)
{
}

IntraDecoder::~IntraDecoder()
{
}

int IntraDecoder::decode(Mat &frame, function<int(int, int, int)> predictor)
{
    int a, b, c, err, frame_cost = 0, n_ch = frame.channels();

    if (n_ch == 3)
    {
        hconcat(Mat::zeros(frame.rows, 1, CV_8UC3), frame, frame);
        vconcat(Mat::zeros(1, frame.cols, CV_8UC3), frame, frame);
    }
    else if (n_ch == 1)
    {
        hconcat(Mat::zeros(frame.rows, 1, CV_8UC1), frame, frame);
        vconcat(Mat::zeros(1, frame.cols, CV_8UC1), frame, frame);
    }
    int optimalM = golomb.decode();
    cout << "optimalM: " << optimalM << endl;

    if (optimalM != 0)
        golomb.set_m(optimalM);

    for (int i = 1; i < frame.rows; i++)
    {
        for (int n = 1; n < frame.cols; n++)
        {
            for (int ch = 0; ch < n_ch; ch++)
            {
                a = frame.ptr<uchar>(i, n - 1)[ch];
                b = frame.ptr<uchar>(i - 1, n)[ch];
                c = frame.ptr<uchar>(i - 1, n - 1)[ch];

                err = golomb.decode();
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
    frame = frame(Rect(1, 1, frame.cols - 1, frame.rows - 1));
    return frame_cost / (frame.rows * frame.cols * frame.channels());
}
