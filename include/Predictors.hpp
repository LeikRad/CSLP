#ifndef PREDICTORS_H
#define PREDICTORS_H

using namespace std;

int JPEG_Predictor_1(int a, int b, int c);

int JPEG_Predictor_2(int a, int b, int c);

int JPEG_Predictor_3(int a, int b, int c);

int JPEG_Predictor_4(int a, int b, int c);

int JPEG_Predictor_5(int a, int b, int c);

int JPEG_Predictor_6(int a, int b, int c);

int JPEG_Predictor_7(int a, int b, int c);

int JPEG_LS(int a, int b, int c);

vector<function<int(int, int, int)>> getPredictors();

#endif // PREDICTORS_H
