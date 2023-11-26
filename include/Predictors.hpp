/**
 * @file Predictors.hpp
 * @brief JPEG predictors for image compression
 *
 * This file contains the declarations of eight JPEG predictors used for image compression.
 */

#ifndef PREDICTORS_H
#define PREDICTORS_H

using namespace std;

/**
 * @brief JPEG Predictor 1
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_1(int a, int b, int c);

/**
 * @brief JPEG Predictor 2
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_2(int a, int b, int c);

/**
 * @brief JPEG Predictor 3
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_3(int a, int b, int c);

/**
 * @brief JPEG Predictor 4
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_4(int a, int b, int c);

/**
 * @brief JPEG Predictor 5
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_5(int a, int b, int c);

/**
 * @brief JPEG Predictor 6
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_6(int a, int b, int c);

/**
 * @brief JPEG Predictor 7
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_Predictor_7(int a, int b, int c);

/**
 * @brief JPEG Lossless predictor
 *
 * @param a Left pixel value
 * @param b Above pixel value
 * @param c Above-left pixel value
 * @return Predicted pixel value
 */
int JPEG_LS(int a, int b, int c);

/**
 * @brief Get all predictors
 *
 * @return Vector of all predictors
 */
vector<function<int(int, int, int)>> getPredictors();

#endif // PREDICTORS_H