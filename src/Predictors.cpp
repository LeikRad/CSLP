#include <iostream>
#include <map>
#include <functional>

int JPEG_Predictor_1(int a, int b, int c)
{
    return a;
}

int JPEG_Predictor_2(int a, int b, int c)
{
    return b;
}

int JPEG_Predictor_3(int a, int b, int c)
{
    return c;
}

int JPEG_Predictor_4(int a, int b, int c)
{
    return a + b - c;
}

int JPEG_Predictor_5(int a, int b, int c)
{
    return a + (b - c) / 2;
}

int JPEG_Predictor_6(int a, int b, int c)
{
    return b + (a - c) / 2;
}

int JPEG_Predictor_7(int a, int b, int c)
{
    return (a + b) / 2;
}

int JPEG_LS(int a, int b, int c)
{
    int pred = a + b - c;

    int mini = std::min(a, b);
    int maxi = std::max(a, b);

    if (c >= maxi)
    {
        pred = mini;
    }
    if (c <= mini)
        pred = maxi;

    return pred;
}

// debug only
std::map<std::string, std::function<int(int, int, int)>> getFuncNames_Predictors()
{
    std::map<std::string, std::function<int(int, int, int)>> predictors;

    predictors["JPEG_Predictor_1"] = JPEG_Predictor_1;
    predictors["JPEG_Predictor_2"] = JPEG_Predictor_2;
    predictors["JPEG_Predictor_3"] = JPEG_Predictor_3;
    predictors["JPEG_Predictor_4"] = JPEG_Predictor_4;
    predictors["JPEG_Predictor_5"] = JPEG_Predictor_5;
    predictors["JPEG_Predictor_6"] = JPEG_Predictor_6;
    predictors["JPEG_Predictor_7"] = JPEG_Predictor_7;
    predictors["JPEG_LS"] = JPEG_LS;

    return predictors;
}

std::vector<std::function<int(int, int, int)>> getPredictors()
{
    std::vector<std::function<int(int, int, int)>> predictors;

    predictors.push_back(JPEG_Predictor_1);
    predictors.push_back(JPEG_Predictor_2);
    predictors.push_back(JPEG_Predictor_3);
    predictors.push_back(JPEG_Predictor_4);
    predictors.push_back(JPEG_Predictor_5);
    predictors.push_back(JPEG_Predictor_6);
    predictors.push_back(JPEG_Predictor_7);
    predictors.push_back(JPEG_LS);

    return predictors;
}