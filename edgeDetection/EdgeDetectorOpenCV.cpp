#include "EdgeDetectorOpenCV.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int EdgeDetectorOpenCV::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
	int rows = sourceImage.rows;
	int cols = sourceImage.cols;
	int channels = sourceImage.channels();
    int ddepth = CV_16S;
	
	if (channels != 1){ 
		cout<<"sourceImage khong phai anh grayScale"<<endl;
		return 1;
	}
	float eps = 1e-6;

	switch (method) {
	case 1: {
		// float threshold = 25;
        GaussianBlur(sourceImage, sourceImage, Size(kWidth, kHeight), 0, 0, BORDER_DEFAULT);
        // Convert the image to grayscale
        // cvtColor(, src_gray, COLOR_BGR2GRAY);
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Sobel(sourceImage, grad_x, ddepth, 1, 0, 1, 1, 0, BORDER_DEFAULT);
        Sobel(sourceImage, grad_y, ddepth, 0, 1, 1, 1, 0, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, destinationImage);
	}
		break;
	case 2: {
		cout << "Prewitt Method \n";
        GaussianBlur(sourceImage, sourceImage, Size(kWidth, kHeight), 0, 0, BORDER_DEFAULT);
        // Convert the image to grayscale
        // cvtColor(, src_gray, COLOR_BGR2GRAY);
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        vector <float> Wx = { -1 , 0, 1, -2, 0, 2, -1, 0, 1 };
		vector <float> Wy = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
		
        filter2D(sourceImage, grad_x, ddepth, Wx, Point(-1, -1), 0, BORDER_DEFAULT);
        filter2D(sourceImage, grad_y, ddepth, Wy, Point(-1, -1), 0, BORDER_DEFAULT);

        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, destinationImage);
		}
		break;
	case 3: {
		cout<<"Laplace"<<endl;
        GaussianBlur( sourceImage, sourceImage, Size(kWidth, kHeight), 0, 0, BORDER_DEFAULT );
        // cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to grayscale
        Mat dst;
        Laplacian(sourceImage, dst, ddepth, 1, 1, 0, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs( dst, destinationImage);
		
		return 0;
	}
		break;
	case 4: {
        cout<<"Canny"<<endl;
        GaussianBlur( sourceImage, sourceImage, Size(kWidth, kHeight), 0, 0, BORDER_DEFAULT );
        Canny(sourceImage, destinationImage, 100, 200);

        return 0;
		}
		break;
	}
	

	return 0;
}

EdgeDetectorOpenCV::EdgeDetectorOpenCV() {
}

EdgeDetectorOpenCV::~EdgeDetectorOpenCV() {
}
