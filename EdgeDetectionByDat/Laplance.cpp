#include "Laplance.h"


void getInput(int& gaussianSize, float& sigma)
{
    cout << "gaussianSize (int): ";
    cin >> gaussianSize;
    cout << "sigma (float): ";
    cin >> sigma;
}

float thresholdCalculate(const Mat& img)
{
    float threshold = 0;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            threshold = img.at<float>(i, j) > threshold ? img.at<float>(i, j) : threshold;
        }
    }

    threshold = threshold > 255 ? 255 : threshold;
    threshold = threshold * 1.0 / 4;

    return threshold;
}

bool isZeroCrossing(const Mat& img, int x, int y, float threshold)
{
    int count = 0;

    for (int i=-1; i<=1; i++)
    {
        for (int j=-1; j<=1 ; j++)
        {
            float value1 = img.at<float>(x-i, y-j);
            float value2 = img.at<float>(x+i, y+j);
            int sign1 = value1 > 0 ? 1 : -1;
            int sign2 = value2 > 0 ? 1 : -1;
            //cout << value1 << " " << value2 << " " << abs(value1 - value2) << " " << threshold << endl;
            if (sign1 != sign2 && abs(value1 - value2) > threshold)
                count ++;
        }
    }

    return count >= 2;
}

bool zeroCrossing(Mat& img, Mat& dest, float threshold)
{
    dest = Mat::zeros(img.size(), img.type());
    for (int i = 1; i < img.rows-1; i++)
    {
        for (int j = 1; j < img.cols-1; j++)
        {
            if(isZeroCrossing(img, i, j, threshold))
                dest.at<float>(i, j) = 255;
            else
                dest.at<float>(i, j) = 0;
        }
    }

    return true;
}

bool detectByLaplance(const Mat& src, Mat& dest, int kernelSize, float sigma)
{
    Mat img = src.clone();
    Mat grad;

    gaussianBlur(img, img, kernelSize, sigma);

    vector<float> kernel = {1,1,1,1,-8,1,1,1,1};

    convolution(img, grad, kernel, 3);

    float threshold = thresholdCalculate(grad);

    zeroCrossing(grad, dest, threshold);

    return true;
}