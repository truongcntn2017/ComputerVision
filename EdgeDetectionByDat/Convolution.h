#pragma once

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
using namespace cv;

bool convolution(const Mat& src, Mat& dest, vector<float> kernel, int size);
bool computeGrad(Mat& grad, Mat&Gx, Mat&Gy, int k, int norm);
bool computeAngle(Mat& angle, Mat& Gx, Mat& Gy);

void showImg(Mat img, string nameImg, bool convert);