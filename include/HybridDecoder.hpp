#ifndef HYBRID_DECODER_HPP
#define HYBRID_DECODER_HPP

#include "Converter.hpp"
#include "Golomb.hpp"
#include "InterDecoder.hpp"
#include "IntraDecoder.hpp"
#include <vector>
#include "Predictors.hpp"
using namespace cv;
using namespace std;

class HybridDecoder
{
private:
    string input_file;
    vector<function<int(int, int, int)>> predictors;

public:
    HybridDecoder(string input_file);
    void decode();
};

#endif // HYBRID_DECODER_HPP