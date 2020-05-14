#include "Blur.h"

vector<float> gaussianKernel(int size, float sigma)
{
    vector<float> kernel(size*size, 0);
    int radius = (int)(size/2);

    for (int i=-radius; i<=radius; i++)
    {
        for (int j=-radius; j<=radius; j++)
        {
            kernel[(i+radius)*size + (j+radius)] = 1.0 * exp(-i*i/(2.0*sigma*sigma) - j*j/(2.0*sigma*sigma)) / (2 * M_PI * sigma * sigma);
        }
    }
    return kernel;
}

bool gaussianBlur(const Mat& src, Mat& dest, int size, float sigma)
{
    Mat img = src.clone();
    //img.convertTo(img, CV_32FC3);

    vector<float> kernel = gaussianKernel(size, sigma);

    convolution(img, dest, kernel, size);

    dest.convertTo(dest, CV_8UC3);

    return true;
}
