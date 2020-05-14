#pragma once

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <math.h>

#include "Convolution.h"
#include "Blur.h"

using namespace std;
using namespace cv;


bool sobelFilters(const Mat& img, Mat& grad, Mat& theta, bool norm=true);
bool normalize(Mat& img);
bool nonMaxSuppression(Mat& img, Mat& angle);
bool doubleThreshold(Mat& img, float lowThreshold=10, float highThreshold=20);
bool isNearStrong(Mat img, int x, int y, float strong);
bool hysteresis(Mat& img, Mat angle, float weak, float strong=255);

bool detectByCanny(const Mat& src, Mat& dest, float lowThreshold=10, float highThreshold=20, int gaussianSize=5, float sigma=3, int norm=1);

void getInput(float& lowThreshold, float& highThreshold, int& gaussianSize, float& sigma, int& norm);
