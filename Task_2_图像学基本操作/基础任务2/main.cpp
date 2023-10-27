#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// 图像双线性插值缩放函数
Mat my_resizeImage(const Mat& input, int newWidth, int newHeight)
{
    Mat output(newHeight, newWidth, input.type());

    for (int y = 0; y < newHeight; y++) 
    {
        for (int x = 0; x < newWidth; x++) 
        {
            float sourceX = (x + 0.5) * (input.cols / (float)(newWidth)) - 0.5;
            float sourceY = (y + 0.5) * (input.rows / (float)(newHeight)) - 0.5;

            int sourceX0 = (int)(sourceX);
            int sourceX1 = min(sourceX0 + 1, input.cols - 1);
            int sourceY0 = (int)(sourceY);
            int sourceY1 = min(sourceY0 + 1, input.rows - 1);

            float alpha = sourceX - sourceX0;
            float beta = sourceY - sourceY0;

            Vec3b pixel00 = input.at<Vec3b>(sourceY0, sourceX0);
            Vec3b pixel01 = input.at<Vec3b>(sourceY0, sourceX1);
            Vec3b pixel10 = input.at<Vec3b>(sourceY1, sourceX0);
            Vec3b pixel11 = input.at<Vec3b>(sourceY1, sourceX1);

            Vec3b interpolatedPixel;
            for (int c = 0; c < 3; c++) {
                interpolatedPixel[c] = static_cast<uchar>((1.0 - alpha) * (1.0 - beta) * pixel00[c] +
                                                          alpha * (1.0 - beta) * pixel01[c] +
                                                          (1.0 - alpha) * beta * pixel10[c] +
                                                          alpha * beta * pixel11[c]);
            }

            output.at<Vec3b>(y, x) = interpolatedPixel;
        }
    }

    return output;
}

// 滑动条回调函数
void myresize(int, void*) 
{
    int ratio = getTrackbarPos("ratio", "Resize");
    double scale = ratio / 100.0;
    Mat src = imread("../743449.jpg");  // 读取图像
    if (src.empty()) {
        cout << "Read Imagine Error!" << endl;
        exit(-1);
    }
    Mat resizedImage = my_resizeImage(src, (int)(src.cols * scale), (int)(src.rows * scale));
    imshow("Resize", resizedImage);
    waitKey(0);
}

int main() {

    namedWindow("Resize");
    int ratio = 10;
    createTrackbar("ratio", "Resize", &ratio, 200, myresize, NULL);

    return 0;
}


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat gaussianKernel;  // 定义高斯核，可以在 main 函数中初始化

// 创建高斯核
Mat Guss(void)
{
    int kernelSize = 5;  // 高斯核大小
    double sigma = 1.0;  // 高斯核标准差

    Mat kernelX = getGaussianKernel(kernelSize, sigma, CV_64F);
    Mat kernelY = getGaussianKernel(kernelSize, sigma, CV_64F);

    Mat py = kernelX * kernelY.t(); 
    return py;
}

// 滑动条回调函数
void myresize(int, void*) 
{
    int ratio = getTrackbarPos("ratio", "Resize");
    double scale = ratio / 100.0;
    Mat src = imread("../743449.jpg");  // 读取图像
    if (src.empty()) 
    {
        cout << "Read Image Error!" << endl;
        return;
    }
    int newrows = scale * src.rows, newcols = scale * src.cols;
    Mat dst = Mat::zeros(int(scale*src.rows),int(scale*src.cols),CV_8UC3);
    for(int i = 0; i < newrows; i ++)
    {
        
        for(int j = 0; j < newcols; j ++)
        {
            int retrivex = i / scale;
            int retrivey = j / scale;
            Vec3b tem = {0, 0, 0};

            for(int k = -2; k < 3; k ++)
            {
                for(int t = -2; t < 3; t ++)
                {
                    if (retrivex + k >= 0 && retrivex + k < src.rows && retrivey + t >= 0 && retrivey + t < src.cols)
                    {
                        for(int c = 0; c < 3; c ++)
                        {
                            tem[c] += src.at<Vec3b>(retrivex + k, retrivey + t)[c] * gaussianKernel.at<double>(2 + k, 2 + t);
                        }
                    }
                }
            }
            for(int c = 0; c < 3; c ++)
                dst.at<Vec3b>(i, j)[c] = (int)tem[c];
        }
    }
    imshow("Resize", dst);

    return ;
}

int main() {
    gaussianKernel = Guss();  // 初始化高斯核

    namedWindow("Resize");
    int ratio = 100;
    createTrackbar("ratio", "Resize", &ratio, 200, myresize, NULL);

    waitKey(0);
    return 0;
}

