#include "CannyEdge.h"


void getInput(float& lowThreshold, float& highThreshold, int& gaussianSize, float& sigma, int& norm)
{
    cout << "gaussianSize (int): ";
    cin >> gaussianSize;
    cout << "sigma (float): ";
    cin >> sigma;
    cout << "low threshold (float): ";
    cin >> lowThreshold;
    cout << "high threshold (float): ";
    cin >> highThreshold;
    cout << "compute grad method (0=abs, 1=norm): ";
    cin >> norm;
}

bool sobelFilters(const Mat& img, Mat& grad, Mat& theta, bool norm)
{
    Mat Gx, Gy;
    vector<float> dx = {-1,0,1,-2,0,2,-1,0,1};
    vector<float> dy = {-1,-2,-1,0,0,0,1,2,1};

    convolution(img, Gx, dx, 3);
    convolution(img, Gy, dy, 3);

    computeGrad(grad, Gx, Gy, 2, norm);
    computeAngle(theta, Gx, Gy);
    
    return true;
}

bool normalize(Mat& img)
{
    float max = 0;

    for (int i=0; i<img.rows; i++)
    {
        for (int j=0; j<img.cols; j++)
        {
            max = img.at<float>(i, j) > max ? img.at<float>(i, j) : max;
        }
    }
    
    for (int i=0; i<img.rows; i++)
    {
        for (int j=0; j<img.cols; j++)
        {
            img.at<float>(i, j) = 1.0 * img.at<float>(i, j) / (float)max * 255;
        }
    }

    return true;
}

bool nonMaxSuppression(Mat& img, Mat& angle)
{
    for (int i=1; i < img.rows-1; i++)
    {
        for (int j=1; j < img.cols-1; j++)
        {
            int l = 255, r = 255;

            if (angle.at<float>(i, j) == 0)
            {
                l = img.at<float>(i, j-1);
                r = img.at<float>(i, j+1);
            }
            else if (angle.at<float>(i, j) == 45)
            {
                l = img.at<float>(i+1, j-1);
                r = img.at<float>(i-1, j+1);
            }
            else if (angle.at<float>(i, j) == 90)
            {
                l = img.at<float>(i+1, j);
                r = img.at<float>(i-1, j);
            }
            else if (angle.at<float>(i, j) == 135)
            {
                l = img.at<float>(i-1, j-1);
                r = img.at<float>(i+1, j+1);
            }
            
            if (img.at<float>(i, j) < l || img.at<float>(i, j) < r)
				img.at<float>(i, j) = 0;
			
        }
    }

    return true;
}

bool doubleThreshold(Mat& img, float lowThreshold, float highThreshold)
{
 
    float strong = 255;
    float weak = 25;

    for (int i=0; i < img.rows; i++)
    {
        for (int j=0; j < img.cols; j++)
        {
            if (img.at<float>(i, j) >= highThreshold)
            {
                img.at<float>(i, j) = strong;
            }
            else if (img.at<float>(i, j) >= lowThreshold)
            {
                img.at<float>(i, j) = weak;
            }
            else
            {
                img.at<float>(i, j) = 0;
            }
            
        }
    }

    return true;
}

bool isNearStrong(Mat img, int x, int y, float strong)
{
    for (int i=-1; i<=1; i++)
    {
        for (int j=-1; j<=1; j++)
        {
            if (img.at<float>(x+i, y+i) == strong)
                return true;
        }
    }

    return false;
}

bool hysteresis(Mat& img, Mat angle, float weak, float strong)
{
    for (int i=1; i < img.rows-1; i++)
    {
        for (int j=1; j < img.cols-1; j++)
        {
            if (img.at<float>(i, j) == weak)
            {
                float l = 0, r = 0;
                if (angle.at<float>(i, j) == 90)
                {
                    l = img.at<float>(i, j-1);
                    r = img.at<float>(i, j+1);
                }
                else if (angle.at<float>(i, j) == 135)
                {
                    l = img.at<float>(i+1, j-1);
                    r = img.at<float>(i-1, j+1);
                }
                else if (angle.at<float>(i, j) == 0)
                {
                    l = img.at<float>(i+1, j);
                    r = img.at<float>(i-1, j);
                }
                else if (angle.at<float>(i, j) == 45)
                {
                    l = img.at<float>(i-1, j-1);
                    r = img.at<float>(i+1, j+1);
                }
                
                img.at<float>(i, j) = (l == strong || r == strong) ? strong : 0;
                //img.at<float>(i, j) = isNearStrong(img, i, j, strong) ? strong : 0;
            }            
        }
    }

    return true;
}


bool detectByCanny(const Mat& src, Mat& dest, float lowThreshold, float highThreshold, int gaussianSize, float sigma, int norm)
{
    Mat img = src.clone();
    Mat angle;

    gaussianBlur(img, img, gaussianSize, sigma);

    sobelFilters(img, dest, angle, norm);

    normalize(dest);
    
    nonMaxSuppression(dest, angle);
    
    doubleThreshold(dest, lowThreshold, highThreshold);
    
    hysteresis(dest, angle, 25, 255);

    return true;
}