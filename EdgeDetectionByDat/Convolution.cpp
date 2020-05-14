#include "Convolution.h"


// Convolution

bool convolution(const Mat& src, Mat& dest, vector<float> kernel, int size)
{
    int radius = (int)(size/2);
    
    dest = Mat::zeros(src.rows - size+1, src.cols - size+1, CV_32FC1);

    for (int x=radius; x < src.rows-radius; x++)
    {
        for (int y=radius; y < src.cols-radius; y++)
        {
            //cout << x << " " << y << endl;
            float result = 0;
            for (int i=-radius; i<=radius; i++)
            {
                for (int j=-radius; j<=radius; j++)
                {   
                    result += 1.0 * src.at<uchar>(x+i, y+j) * (kernel[(radius-i)*size + (radius-j)]);
                }
            }

            dest.at<float>(x-1, y-1) = result;
        }
        
    }
    
    return true;
}

bool computeGrad(Mat& grad, Mat&Gx, Mat&Gy, int k, int norm)
{
    grad = Mat::zeros(Gx.size(), Gx.type());

    for (int i = 0; i < grad.rows; i++)
    {
        for (int j = 0; j < grad.cols; j++)
        {
            Gx.at<float>(i, j) /= (k+2);
            Gy.at<float>(i, j) /= (k+2);

            if (norm != 0)
                grad.at<float>(i, j) = sqrt(Gx.at<float>(i, j)*Gx.at<float>(i, j) + Gy.at<float>(i, j)*Gy.at<float>(i, j));
            else
                grad.at<float>(i, j) = abs(Gx.at<float>(i, j)) + abs(Gy.at<float>(i, j));
        }
    }

    return true;
}

bool computeAngle(Mat& angle, Mat& Gx, Mat& Gy)
{
    angle = Mat::zeros(Gx.size(), Gx.type());

    for (int i = 0; i < angle.rows; i++)
    {
        for (int j = 0; j < angle.cols; j++)
        {
            float arctan = atan(Gy.at<float>(i, j)*1.0/Gx.at<float>(i, j)) * 180.0 / M_PI;

            arctan = arctan >= 0 ? arctan : (arctan + 180);

            if ((arctan >= 0 && arctan < 22.5) || (arctan >= 157.5 && arctan <= 180))
                angle.at<float>(i, j) = 0;
            else if(arctan >= 22.5 && arctan < 67.5)
                angle.at<float>(i, j) = 45;
            else if(arctan >= 67.5 && arctan < 112.5)
                angle.at<float>(i, j) = 90;
            else if(arctan >= 112.5 && arctan < 157.5)
                angle.at<float>(i, j) = 135;

        }
    }

    return true;
    
}

void showImg(Mat img, string nameImg, bool convert)
{
    if (convert == true)
        img.convertTo(img, CV_8UC3);
    namedWindow(nameImg, WINDOW_AUTOSIZE); // Give the name to window contains the image
    imshow(nameImg, img); // show the image to the user

}