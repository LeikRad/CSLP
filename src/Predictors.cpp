#include <iostream>
#include <map>
#include <functional>
#include "Predictors.hpp"

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

    int mini = min(a, b);
    int maxi = max(a, b);

    if (c >= maxi)
    {
        pred = mini;
    }
    if (c <= mini)
        pred = maxi;

    return pred;
}

vector<function<int(int, int, int)>> getPredictors()
{
    vector<function<int(int, int, int)>> predictors;

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