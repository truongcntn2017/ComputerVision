#pragma once

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <math.h>

#include "Convolution.h"

using namespace std;
using namespace cv;

vector<float> gaussianKernel(int size, float sigma);
bool gaussianBlur(const Mat& src, Mat& dest, int size, float sigma);