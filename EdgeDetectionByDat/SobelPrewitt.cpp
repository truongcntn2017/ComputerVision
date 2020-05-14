#include "SobelPrewitt.h"


void getInput(int& gaussianSize, float& sigma, int& norm, float& threshold)
{
    cout << "gaussianSize (int): ";
    cin >> gaussianSize;
    cout << "sigma (float): ";
    cin >> sigma;
    cout << "threshold (float): ";
    cin >> threshold;
    cout << "compute grad method (0=abs, 1=norm): ";
    cin >> norm;
}

bool thresholding(Mat& img, float threshold)
{
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            img.at<float>(i, j) = img.at<float>(i, j) > threshold ? 255 : 0;
        }
    }

    return true;
}

bool detectBySobel(const Mat& src, Mat& dest, int kernelSize, float sigma, int norm, float threshold)
{
    Mat img = src.clone();

    gaussianBlur(img, img, kernelSize, sigma);

    Mat Gx, Gy;
    vector<float> dx = {-1,0,1,-2,0,2,-1,0,1};
    vector<float> dy = {-1,-2,-1,0,0,0,1,2,1};

    convolution(img, Gx, dx, 3);
    convolution(img, Gy, dy, 3);

    computeGrad(dest, Gx, Gy, 2, norm);

    thresholding(dest, threshold);

    showImg(Gx, "Gradient X by Sobel", true);
    showImg(Gy, "Gradient Y by Sobel", true);

    return true;
}

bool detectByPrewitt(const Mat& src, Mat& dest, int kernelSize, float sigma, int norm, float threshold)
{
    Mat img = src.clone();

    gaussianBlur(img, img, kernelSize, sigma);

    Mat Gx, Gy;
    vector<float> dx = {-1,0,1,-1,0,1,-1,0,1};
    vector<float> dy = {-1,-1,-1,0,0,0,1,1,1};

    convolution(img, Gx, dx, 3);
    convolution(img, Gy, dy, 3);

    computeGrad(dest, Gx, Gy, 1, norm);

    thresholding(dest, threshold);

    showImg(Gx, "Gradient X by Prewitt", true);
    showImg(Gy, "Gradient Y by Prewitt", true);

    return true;
}