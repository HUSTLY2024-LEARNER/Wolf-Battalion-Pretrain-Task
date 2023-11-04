#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

void gamma_correction(Vec3b& pixel, float gamma)
{
    for (int i = 0; i < 3; i++) {
        pixel[i] = (unsigned char)(pow(pixel[i] / 255.0, gamma) * 255.0);
    }
}

int main()
{
    Mat src = imread("/mnt/c/Users/86153/Desktop/RM/L_2/img/车牌3.png");

    if (src.empty())
    {
        cout << "read false!" << endl;
        exit(-1);
    }

    Mat dst = src.clone();
    float gamma = 2.2; 

    imshow("before gamma correction", src);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            Vec3b &pixel = dst.at<Vec3b>(i, j);
            gamma_correction(pixel, gamma);
        }
    }

    imshow("gamma correction", dst);
    waitKey(0);

    return 0;
}
