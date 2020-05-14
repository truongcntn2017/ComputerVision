#include <opencv4/opencv2/core.hpp>
#include "opencv4/opencv2/highgui/highgui.hpp"
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "SobelPrewitt.h"
#include "Laplace.h"
#include "CannyEdge.h"

using namespace std;
using namespace cv;

void help()
{
    cout << "Image Edge Detection by 1712329, 1712195" << endl;
    cout << "how to use: <app> <image-path> <flag-method>" << endl;
    cout << "'-sobel' for edge detection using sobel method" << endl;
    cout << "'-prewitt' for edge detection using prewitt method" << endl;
    cout << "'-laplace' for edge detection using laplace method" << endl;
    cout << "'-canny' for edge detection using canny method" << endl;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        help();
        return -1;
    }

    Mat image; // create a variable with Mat type to contain the image need to be processed
    image = imread(argv[1], IMREAD_COLOR); // open the image from the path

    Mat grayScale = Mat::zeros( image.size(), image.type() );
    cvtColor(image, grayScale, COLOR_RGBA2GRAY) ;

    if (!image.data)
    {
        cout << "Cannot open image" << std::endl;
        return -1;
    }
    
    Mat new_image; //creat a mat to contain image after processing

    int gaussianSize, norm;
    float sigma, lowThreshold, highThreshold;


    if(strcmp(argv[2], "-sobel") == 0)
    {
        getInput(gaussianSize, sigma, norm, highThreshold);
        detectBySobel(grayScale, new_image, gaussianSize, sigma, norm, highThreshold);
    }
    else if (strcmp(argv[2], "-prewitt") == 0)
    {
        getInput(gaussianSize, sigma, norm, highThreshold);
        detectByPrewitt(grayScale, new_image, gaussianSize, sigma, norm, highThreshold);
    }
    else if (strcmp(argv[2], "-laplace") == 0)
    {
        getInput(gaussianSize, sigma);
        detectByLaplace(grayScale, new_image, gaussianSize, sigma);
    }
    else if (strcmp(argv[2], "-canny") == 0)
    {
        getInput(lowThreshold, highThreshold, gaussianSize, sigma, norm);
        detectByCanny(grayScale, new_image, lowThreshold, highThreshold, gaussianSize, sigma, norm);
    }
    else
    {
        help();
    }
    
    
    showImg(image, "Origional Image", false);
    showImg(new_image, "Edge Detected Image", false);

    waitKey(0);
    return 0;
}