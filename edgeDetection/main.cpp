#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "EdgeDetector.h"
#include "Convolution.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
    string fname = argc[1];
    Mat image = imread(fname, IMREAD_COLOR);
    Mat edgeImage = Mat::zeros( image.size(), image.type() );
    if (!image.data){
        cout<<"Khong the mo anh"<<endl;		//imshow("Show Image", Gx);
		//imshow("Show Image", Gy);
        return -1;
    }


    Mat grayScale = Mat::zeros( image.size(), image.type() );
    cvtColor(image, grayScale, COLOR_RGBA2GRAY) ;
    
    EdgeDetector edgeDetector;
    int method = atoi(argc[2]);
    cout<<edgeDetector.DetectEdge(grayScale, edgeImage, 3, 3, method)<<endl;    
    
    namedWindow("Grayscale image", WINDOW_AUTOSIZE); // (3)
    imshow("Grayscale image", grayScale); // (4)
    namedWindow("Edge image", WINDOW_AUTOSIZE); // (3)
    imshow("Edge image", edgeImage); // (4)
    waitKey(0);

    return 0;
}