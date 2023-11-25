#ifndef PREDICTORS_H
#define PREDICTORS_H
int JPEG_Predictor_1(int a, int b, int c);

int JPEG_Predictor_2(int a, int b, int c);

int JPEG_Predictor_3(int a, int b, int c);

int JPEG_Predictor_4(int a, int b, int c);

int JPEG_Predictor_5(int a, int b, int c);

int JPEG_Predictor_6(int a, int b, int c);

int JPEG_Predictor_7(int a, int b, int c);

int JPEG_LS(int a, int b, int c);

std::map<std::string, std::function<int(int, int, int)>> getFuncNames_Predictors_vec();

std::vector<std::function<int(int, int, int)>> getPredictors();

#endif // PREDICTORS_H
