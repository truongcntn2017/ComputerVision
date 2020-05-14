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

bool thresholding(Mat& img, float threshold);

bool detectBySobel(const Mat& src, Mat& dest, int gaussianlSize=3, float sigma=1, int norm=1, float threshold=15);
bool detectByPrewitt(const Mat& src, Mat& dest, int gaussianSize=3, float sigma=1, int norm=1, float threshold=15);

void getInput(int& gaussianSize, float& sigma, int& norm, float& threshold);