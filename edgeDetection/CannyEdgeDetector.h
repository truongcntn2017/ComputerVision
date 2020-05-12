#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <math.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using std::vector;
using std::pair;

class CannyEdgeDetector
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;

public:
	Mat doconvolve(const Mat&, const Mat&);
	Mat gaussian_kernel(int, float);
	void sobel_filters(const Mat&, Mat&, Mat&);
	Mat non_max_suppression(const Mat&, const Mat&);
	Mat hysteresis(const Mat&);
	void threshold(Mat&);
	void setter(int low, int high)
	{
		_lowThreshold = low;
		_highThreshold = high;
	}


	/*
		Hàm áp dụng thuật toán Canny để phát hiện biên cạnh
		- srcImage: ảnh input
		- dstImage: ảnh kết quả
		Hàm trả về
			1: nếu detect thành công
			0: nếu detect không thành công
	*/

	int Apply(const Mat& srcImage, Mat& dstImage);

	CannyEdgeDetector() {};
	~CannyEdgeDetector() {};
};

