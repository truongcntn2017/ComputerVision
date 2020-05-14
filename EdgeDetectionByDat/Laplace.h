#pragma once

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <math.h>

#include "Convolution.h"
#include "Blur.h"

using namespace cv;
using namespace std;

float thresholdCalculate(const Mat& img);
bool isZeroCrossing(const Mat& img, int x, int y, float threshold);
bool zeroCrossing(Mat& img, Mat& dest, float threshold);

bool detectByLaplace(const Mat& src, Mat& dest, int gaussianSize=3, float sigma=1);

void getInput(int& gaussianSize, float& sigma);