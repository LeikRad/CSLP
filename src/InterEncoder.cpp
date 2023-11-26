#include "InterEncoder.hpp"
#include <unistd.h>

InterEncoder::InterEncoder(GolombEncoder &golomb, int block_size, int block_range, int shift) : golomb(golomb), block_size(block_size), block_range(block_range), shift(shift)
{
}

int InterEncoder::getBlockSize() const
{
    return block_size;
}

int InterEncoder::getBlockRange() const
{
    return block_range;
}

void InterEncoder::setBlockSize(int block_size)
{
    block_size = block_size;
}

float InterEncoder::cost(Mat block)
{
    return sum(sum(abs(block)))[0];
}

int InterEncoder::encode(Mat &old_frame, Mat &new_frame)
{

    Mat block_diff;
    Mat min_block_diff;
    Mat aux_frame;

    int n_ch = new_frame.channels();

    // these are concatenating a row of zeros to the upwards and left side of the array;
    if (n_ch == 3)
    {
        block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
        aux_frame = Mat::zeros(new_frame.rows, new_frame.cols, CV_16SC3);
    }
    else if (n_ch == 1)
    {
        block_diff = Mat::zeros(block_size, block_size, CV_16SC1);
        aux_frame = Mat::zeros(new_frame.rows, new_frame.cols, CV_16SC1);
    }
    else
    {
        cout << "Error: Invalid number of channels" << endl;
        exit(1);
    }

    int min_diff, min_block_sum, mEnc, err, min_x, min_y, framecost = 0, count = 0;
    int max_x = (new_frame.cols - block_size), max_y = (new_frame.rows - block_size);
    int location[(new_frame.rows * new_frame.cols) / (block_size * block_size) * 2];

    for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
    {
        for (int curr_x = 0; curr_x <= max_x; curr_x += block_size)
        {
            min_diff = 1000000000;

            Mat curr = Mat(new_frame, Rect(curr_x, curr_y, block_size, block_size));

            for (int prev_y = ((curr_y - block_range < 0) ? 0 : (curr_y - block_range));
                 prev_y <= ((curr_y + block_range >= max_y) ? max_y : (curr_y + block_range));
                 prev_y++)
            {
                for (int prev_x = ((curr_x - block_range < 0) ? 0 : (curr_x - block_range));
                     prev_x <= ((curr_x + block_range >= max_x) ? max_x : (curr_x + block_range));
                     prev_x++)
                {
                    Mat prev = Mat(old_frame, Rect(prev_x, prev_y, block_size, block_size));

                    if (n_ch == 1)
                        subtract(curr, prev, block_diff, noArray(), CV_16SC1);
                    else if (n_ch == 3)
                        subtract(curr, prev, block_diff, noArray(), CV_16SC3);
                    else
                    {
                        cout << "Error: Invalid number of channels" << endl;
                        exit(1);
                    }

                    min_block_sum = this->cost(block_diff);
                    if (min_block_sum < min_diff)
                    {
                        min_x = prev_x;
                        min_y = prev_y;
                        block_diff.copyTo(min_block_diff);
                        min_diff = min_block_sum;
                        if (min_diff < 1000)
                            break;
                    }
                }
                if (min_diff < 1000)
                    break;
            }
            location[count++] = min_x;
            location[count++] = min_y;

            Rect rect(curr_x, curr_y, block_size, block_size);
            min_block_diff.copyTo(aux_frame(rect));
        }
    }

    mEnc = golomb.optimal_m(aux_frame);

    if (golomb.get_m() == mEnc)
    {
        golomb.encode(0);
    }
    else
    {
        golomb.encode(mEnc);
        golomb.set_m(mEnc);
    }

    count = 0;

    for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
    {
        for (int curr_x = 0; curr_x <= max_x; curr_x += block_size)
        {
            golomb.encode(location[count++]);
            golomb.encode(location[count++]);

            for (int i = curr_y; i < curr_y + block_size; i++)
            {
                for (int j = curr_x; j < curr_x + block_size; j++)
                {
                    for (int ch = 0; ch < n_ch; ch++)
                    {
                        err = aux_frame.ptr<short>(i, j)[ch];
                        if (err < 0)
                        {
                            err = -1 * (abs(err) >> shift);
                        }
                        else
                        {
                            err >>= shift;
                        }
                        golomb.encode(err);
                        framecost += abs(err);

                        if (err < 0)
                            err = -1 * (abs(err) << shift);
                        else
                            err <<= shift;

                        new_frame.ptr<uchar>(i, j)[ch] = (unsigned char)old_frame.ptr<uchar>(i, j)[ch] + err;
                    }
                }
            }
        }
    }

    return framecost / (new_frame.rows * new_frame.cols * n_ch);
}