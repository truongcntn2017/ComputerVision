#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "EdgeDetector.h"
#include "Convolution.h"
#include "EdgeDetectorOpenCV.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
    string fname = argc[1];
    Mat image = imread(fname, IMREAD_COLOR);
    if (!image.data){
        cout<<"Khong the mo anh"<<endl;		//imshow("Show Image", Gx);
		//imshow("Show Image", Gy);
        return -1;
    }


    Mat grayScale = Mat::zeros( image.size(), image.type() );
    Mat edgeImage = Mat::zeros( image.size(), image.type() );
    Mat edgeImageOpenCV = Mat::zeros( image.size(), image.type() );

    cvtColor(image, grayScale, COLOR_RGBA2GRAY) ;
    EdgeDetector edgeDetector;
    EdgeDetectorOpenCV edgeDetectorOpenCV;

    int method = atoi(argc[2]);
    int kWidth = atoi(argc[3]);
    int kHeight = atoi(argc[4]);

    edgeDetector.DetectEdge(grayScale, edgeImage, kWidth, kHeight, method);
    edgeDetectorOpenCV.DetectEdge(grayScale, edgeImageOpenCV, kWidth, kHeight, method);
    
    // Mat img8bit;
    // edgeImage.convertTo(img8bit, CV_8UC3, 255.0);
    // imwrite("google-canny.png", img8bit);
    // imwrite("google-canny-opencv.png",edgeImageOpenCV);

    namedWindow("Grayscale image", WINDOW_AUTOSIZE); // (3)
    imshow("Grayscale image", grayScale); // (4)
    namedWindow("Edge detection from our coding", WINDOW_AUTOSIZE); // (5)
    imshow("Edge detection from our coding", edgeImage); // (6)
    namedWindow("Edge detection from OpenCV", WINDOW_AUTOSIZE); // (7)
    imshow("Edge detection from OpenCV", edgeImageOpenCV); // (8)
    waitKey(0);

    return 0;
}