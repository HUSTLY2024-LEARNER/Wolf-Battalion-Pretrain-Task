/*https://blog.csdn.net/shandianfengfan/article/details/120600453*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat inputImg = imread("../743449.jpg");
    if (inputImg.empty())
    {
        cout << "Error: Could not read the image." << endl;
        exit(-1);
    }

    Mat hsvImg(inputImg.size(), CV_8UC3);  // Create an empty HSV image with the same size as the input image
	Mat grayImg(inputImg.size(), CV_8UC1);
    for (int i = 0; i < inputImg.rows; i++)
    {
        for (int j = 0; j < inputImg.cols; j++)
        {
			Vec3b tem, res;
            tem = inputImg.at<Vec3b>(i, j);
			/*HSV*/
			int m, n;
			m = max(max(tem[0], tem[1]), tem[2]);
			n = min(min(tem[0], tem[1]), tem[2]);
			res[2] = m; //V
			int x = m - n; //max - min
			if(x)
			{
				res[1] = x / m;
			}
			else res[1] = 0; //S
			if (m == n)
                res[0] = 0; // specific
			else
			{
				if(m == tem[2])
				{
					res[0] = 60*(tem[1] - tem[0]) / x;
				}
				else if(m == tem[1])
				{
					res[0] = 120 + 60*(tem[0] - tem[2]) / x;
				}
				else res[0] = 240 + 60*(tem[2] - tem[1]) / x;

			}
			
			if(res[0] < 0) res[0] += 360; //H

			res[0] /= 2; res[1] *= 255; res[2] *= 255;
			hsvImg.at<Vec3b>(i, j) = res;
			/*GRAY*/
			int Gray =  (tem[2]*299 + tem[1]*587 + tem[0]*114 + 500) / 1000;
			grayImg.at<uchar>(i, j) = Gray;
		}
    }

	imshow("GRAY_Img", grayImg);
    imshow("HSV_Img", hsvImg);
    waitKey(0);
}
