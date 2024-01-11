#include "opencv2/opencv.hpp"
#include "Golomb.hpp"
#include "HybridEncoder.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[])
{
    string output;
    string input;
    int predictor = -1;
    int block_range = 5;
    int period = 1;
    int shift = 0;

    for(int i = 1; i < argc; i++){
        string arg(argv[i]);
        if (arg == "-o")
        {
            output = argv[i + 1];
        }
        else if (arg == "-i")
        {
            input = argv[i + 1];
        }
        else if (arg == "-p")
        {
            predictor = atoi(argv[i + 1]);
            if (predictor < 0 || predictor > 7)
            {
                cout << "Invalid predictor" << endl;
                return 1;
            }
        }
        else if (arg == "-b")
        {
            block_range = atoi(argv[i + 1]);
            if (block_range < 1)
            {
                cout << "Invalid block range" << endl;
                return 1;
            }
            if (block_range > 10)
            {
                cout << "Warning: block range is greater than 10, perfomance issues may arise." << endl;
            }
            
        }
        else if (arg == "-s")
        {   
            shift = atoi(argv[i + 1]);
            if (shift < 0)
            {
                cout << "Invalid shift" << endl;
                return 1;
            }

            if (shift > 4)
            {
                cout << "Warning: shift is greater than 4, quality issues may arise." << endl;
            }
        }
        else if (arg == "-r")
        {
            period = atoi(argv[i + 1]);
            if (period < 1)
            {
                cout << "Invalid period" << endl;
                return 1;
            }
        }
    }

    // assert that all arguments are present
    if (output.empty() || input.empty() || predictor == -1)
    {
        cout << "Usage: " << argv[0] << " -i <input file> -o <output file> -p <predictor> [-b <block range> default = 5] [-s <shift> default = 0 (lossless)] [-r <period> default = 1 (intra encoder only)]" << endl;
        cout << "[] are optional arguments." << endl;
        return 1;
    }

    HybridEncoder encoder(input, predictor, period, block_range, shift);

    auto start = chrono::high_resolution_clock::now();

    encoder.encode(output);

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms" << endl;
    return 0;
}
