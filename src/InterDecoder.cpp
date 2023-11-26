#include "InterDecoder.hpp"

InterDecoder::InterDecoder(GolombDecoder &golomb, int block_size, int block_range, int shift) : golomb(golomb), block_size(block_size), block_range(block_range), shift(shift)
{
}

int InterDecoder::decode(Mat &old_frame, Mat &new_frame)
{

    Mat old_block, new_block;

    int max_x = new_frame.cols - block_size, max_y = new_frame.rows - block_size;

    int old_x, old_y, err, frame_cost = 0;

    golomb.set_m(golomb.decode());

    for (int curr_y = 0; curr_y <= max_y; curr_y += block_size)
    {
        for (int curr_x = 0; curr_x <= max_x; curr_x += block_size)
        {
            old_x = golomb.decode();
            old_y = golomb.decode();

            for (int i = curr_y; i < curr_y + block_size; i++)
            {
                for (int j = curr_x; j < curr_x + block_size; j++)
                {
                    for (int ch = 0; ch < new_frame.channels(); ch++)
                    {
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

                        new_frame.ptr<uchar>(i, j)[ch] = old_frame.ptr<uchar>(i + old_y - curr_y, j + old_x - curr_x)[ch] + err;
                    }
                }
            }
        }
    }

    return frame_cost / (new_frame.rows * new_frame.cols * new_frame.channels());
}