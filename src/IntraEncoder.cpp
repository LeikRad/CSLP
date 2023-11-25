#include "BitStream.h"
#include "IntraEncoder.h"

IntraEncoder::IntraEncoder(Golomb &golomb, BitStream &bs, int shift) : golomb(golomb), bs(bs), shift(shift)
{
}

IntraEncoder::~IntraEncoder()
{
}

int IntraEncoder::encode(cv::Mat &frame, std::function<int(int, int, int)> predictor)
{

    if (!bs.can_write())
    {
        std::cerr << "Error: BitStream is not ready to read or write" << std::endl;
        return -1;
    }

    int a, b, c, err, pred, channels, mEnc, framecost, n_ch, size;
    cv::Mat image, errorMat;

    n_ch = frame.channels();

    // these are concatenating a row of zeros to the upwards and left side of the array;
    if (n_ch == 3)
    {
        hconcat(cv::Mat::zeros(frame.rows, 1, CV_8UC3), frame, image);
        vconcat(cv::Mat::zeros(1, frame.cols + 1, CV_8UC3), image, image);
        errorMat = cv::Mat::zeros(frame.rows, frame.cols, CV_16SC3);
    }
    else if (n_ch == 1)
    {
        hconcat(cv::Mat::zeros(frame.rows, 1, CV_8UC1), frame, image);
        vconcat(cv::Mat::zeros(1, frame.cols + 1, CV_8UC1), image, image);
        errorMat = cv::Mat::zeros(frame.rows, frame.cols, CV_16SC1);
    }
    else
    {
        std::cout << "Error: Invalid number of channels" << std::endl;
        exit(1);
    }

    size = frame.rows * frame.cols * n_ch;
    for (int i = 1; i < image.rows; i++)
    {
        for (int n = 1; n < image.cols; n++)
        {
            for (int ch = 0; ch < n_ch; ch++)
            {
                a = image.ptr<uchar>(i, n - 1)[ch];

                b = image.ptr<uchar>(i - 1, n)[ch];

                c = image.ptr<uchar>(i - 1, n - 1)[ch];

                pred = predictor(a, b, c);
                err = image.ptr<uchar>(i, n)[ch] - pred;

                if (err < 0)
                {
                    err = -1 * (abs(err) >> shift);
                }
                else
                {
                    err >>= shift;
                }
                // Store Error = estimate - real value.
                errorMat.ptr<short>(i - 1, n - 1)[ch] = err;

                if (err < 0)
                    err = -1 * (abs(err) << shift);
                else
                    err <<= shift;
                image.ptr<uchar>(i, n)[ch] = (unsigned char)pred + err;
            }
        }
    }

    mEnc = golomb.optimal_m(errorMat);
    if (golomb.getM() == mEnc)
    {
        golomb.Encode(0);
    }
    else
    {
        golomb.Encode(mEnc);
        golomb.setM(mEnc);
    }

    for (int i = 0; i < errorMat.rows; i++)
        for (int n = 0; n < errorMat.cols; n++)
            for (int ch = 0; ch < n_ch; ch++)
            {
                framecost += abs(errorMat.ptr<short>(i, n)[ch]);
                golomb.Encode(errorMat.ptr<short>(i, n)[ch]);
            }

    int errorW, errorH, imageW, imageH;

    return framecost / size;
}
