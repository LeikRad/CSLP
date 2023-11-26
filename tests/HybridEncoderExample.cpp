#include "opencv2/opencv.hpp"
#include "Golomb.hpp"
#include "HybridEncoder.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[])
{
    cout
        << "Enter the name of the file to save to (absolute path): ";

    string output;

    cin >> output;

    cout << "Enter the name of the file to read from (absolute path): ";

    string input;

    cin >> input;

    cout << "Enter the predictor to use (0-7): ";

    int predictor;

    cin >> predictor;

    int block_range = 5;

    int shift = 0;

    HybridEncoder encoder(input, predictor, 20, block_range, shift);

    auto start = chrono::high_resolution_clock::now();

    encoder.encode(output);

    auto end = chrono::high_resolution_clock::now();

    cout << "Encoding took "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " milliseconds" << endl;

    return 0;
}
