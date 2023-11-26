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

void InterEncoder::setBlockSize(int block_s)
{
    block_size = block_s;
}

float InterEncoder::cost(Mat block)
{
    return sum(sum(abs(block)))[0];
}

int InterEncoder::encode(Mat &old_frame, Mat &curr_frame)
{
    /** Block with error values. */
    Mat block_diff;
    /** Block with error values which minizes min_diff. */
    Mat min_block_diff;
    /** Frame that will store error values so they can later be processed and encoded. */
    Mat aux_frame;
    /** Number of frame channels. */
    int n_ch = curr_frame.channels();

    if (n_ch == 3)
    {
        block_diff = Mat::zeros(block_size, block_size, CV_16SC3);
        aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC3);
    }
    else if (n_ch == 1)
    {
        block_diff = Mat::zeros(block_size, block_size, CV_16SC1);
        aux_frame = Mat::zeros(curr_frame.rows, curr_frame.cols, CV_16SC1);
    }
    /** Sum of matrix that corresponds to the minimum difference between current frame's block
     *	and previous frame's block. */
    int min_diff;
    /** Absolute value of difference between previous frame's block a current fram'es block. */
    int min_block_sum;
    /** Total frame cost. */
    int frame_cost = 0;
    /** Parameter m used for encoding given frame. */
    int mEnc;
    /** Error value (predictor - real value). */
    int err;

    /** x coordinate of previous frame's block which minimizes error. */
    int min_x;
    /** y coordinate of previous frame's block which minimizes error. */
    int min_y;
    /** Max value of x which the sliding block can reach. */
    int max_x = (curr_frame.cols - block_size);
    /** Max value of y which the sliding block can reach. */
    int max_y = (curr_frame.rows - block_size);
    /** Block location vector. */
    int locations[(curr_frame.rows * curr_frame.cols) / (block_size * block_size) * 2];
    /** Current vector position. */
    int count = 0;

    // Iterate through current frame's blocks.
    for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
    {
        for (int curr_x = 0; curr_x <= max_x; curr_x += block_size)
        {
            min_diff = 10000000;

            Mat curr = Mat(curr_frame, Rect(curr_x, curr_y, block_size, block_size));
            // Iterate through previous frame's blocks
            for (int old_y = ((curr_y - block_range < 0) ? 0 : (curr_y - block_range));
                 old_y <= ((curr_y + block_range >= max_y) ? max_y : (curr_y + block_range)); old_y++)
            {
                for (int old_x = ((curr_x - block_range < 0) ? 0 : (curr_x - block_range));
                     old_x <= ((curr_x + block_range >= max_x) ? max_x : (curr_x + block_range)); old_x++)
                {
                    Mat old = Mat(old_frame, Rect(old_x, old_y, block_size, block_size));

                    if (n_ch == 1)
                        subtract(curr, old, block_diff, noArray(), CV_16SC1);
                    else if (n_ch == 3)
                        subtract(curr, old, block_diff, noArray(), CV_16SC3);

                    min_block_sum = this->cost(block_diff);
                    if (min_block_sum < min_diff)
                    {
                        min_x = old_x;
                        min_y = old_y;
                        block_diff.copyTo(min_block_diff);
                        min_diff = min_block_sum;
                        // If the difference between blocks is 0, no need to keep searching.
                        if (min_diff < 1000)
                            break;
                    }
                }
                // If the difference between blocks is 0, no need to keep searching.
                if (min_diff < 1000)
                    break;
            }
            // Store blocks coordinates.
            locations[count++] = min_x;
            locations[count++] = min_y;

            // Store error between blocks
            Rect rect(curr_x, curr_y, block_size, block_size);
            min_block_diff.copyTo(aux_frame(rect));
        }
    }
    // Calculate optimal m...
    mEnc = golomb.optimal_m(aux_frame);

    // If m stays the same encode 0, otherwise, encode m
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
        for (int curr_x = 0; curr_x <= max_x; curr_x += block_size)
        {
            golomb.encode(locations[count++]);
            golomb.encode(locations[count++]);
            for (int i = curr_y; i < curr_y + block_size; i++)
                for (int j = curr_x; j < curr_x + block_size; j++)
                    for (int ch = 0; ch < n_ch; ch++)
                    {
                        err = aux_frame.ptr<short>(i, j)[ch];

                        if (err < 0)
                            err = -1 * (abs(err) >> shift);
                        else
                            err >>= shift;

                        golomb.encode(err);
                        frame_cost += abs(err);

                        if (err < 0)
                            err = -1 * (abs(err) << shift);
                        else
                            err <<= shift;

                        curr_frame.ptr<uchar>(i, j)[ch] = err + old_frame.ptr<uchar>(i, j)[ch];
                    }
        }
    return frame_cost / (curr_frame.rows * curr_frame.cols * n_ch);
}