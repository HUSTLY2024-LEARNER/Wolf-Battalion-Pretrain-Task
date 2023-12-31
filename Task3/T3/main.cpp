#include <iostream>
#include <opencv2/opencv.hpp>

#define HALF_LENGTH 33.75
#define HALF_HIGH 13.25

using namespace std;
using namespace cv;

Point2f getMidpoint(Point2f A, Point2f B, Point2f C, Point2f D)
{
    Point2f P;
    P.x = (A.x + B.x + C.x + D.x) / 4;
    P.y = (A.y + B.y + C.y + D.y) / 4;
    return P;
}

void pnp(vector<Point2f>& pnts, Mat cam, Mat dis)
{
        vector<Point3f> obj = vector<Point3f>
        {
            Point3f(-HALF_LENGTH, -HALF_HIGH, 0), // tl
            Point3f(HALF_LENGTH, -HALF_HIGH, 0),  // tr
            Point3f(HALF_LENGTH, HALF_HIGH, 0),   // br
            Point3f(-HALF_LENGTH, HALF_HIGH, 0)   // bl
        };

        Mat rVec = cv::Mat::zeros(3, 3, CV_64FC1); // init rvec
        Mat tVec = cv::Mat::zeros(3, 1, CV_64FC1); // init tvec
        solvePnP(obj, pnts, cam, dis, rVec, tVec);
        cout << " [rVec] ->" << endl <<  rVec << endl;
        cout << " [tVec] ->" << endl << tVec <<endl;
    return;
}


int main()
{
    Mat src = imread("../img/1_whole.png");
    if(src.empty())
    {
        cout << "Read Failed !"  << endl;
        exit(-1);
    }
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    Scalar lower_blue = Scalar(100, 120, 100);
    Scalar upper_blue = Scalar(190, 255, 255);

    Mat blue_mask;
    inRange(hsv, lower_blue, upper_blue, blue_mask);

    imshow("blue_mask", blue_mask);

    Mat cameraMatrix = Mat(3, 3, CV_64F);
    cameraMatrix.at<double>(0, 0) = 1900;  // fx
    cameraMatrix.at<double>(1, 1) = 1900;  // fy
    cameraMatrix.at<double>(0, 2) = 960;   // cx
    cameraMatrix.at<double>(1, 2) = 540;   // cy
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);

    vector<vector<Point>> contours;
    findContours(blue_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat image_with_line = src.clone();

    vector<vector<Point2f>> vertex;//一堆矩形框 
    
    vector<Point2f> pnts;//一堆点对
    for (size_t i = 0; i < contours.size(); i++)
        {
            Scalar color = Scalar(0, 0, 255); // 红色

            RotatedRect rect = minAreaRect(contours[i]);
            Point2f vertices[4];
            // decodedObject.location.push_back(vertices);
            //vertex.push_back(vertices);
            rect.points(vertices);
            //vertex.push_back(vertices);
            vector<Point2f> vertex1; // 使用 vector<Point2f> 来存储矩形的顶点
            for (int j = 0; j < 4; j++)
            {
                line(image_with_line, vertices[j], vertices[(j + 1) % 4], color, 2); // 用red框出轮廓
                vertex1.push_back(vertices[j]);
            }
            vertex.push_back(vertex1); // 将存储顶点的向量添加到 vertex 中
        }
    imshow("result", image_with_line);
    waitKey(0);
    Point2f target;
    
    for(int i = 0; i < vertex.size(); i ++) //外层向量中包含的内层向量的数量
    {
        for(int j = 0; j < vertex.size(); j ++)
        {   
            if(j <= i) continue;
            double x1 = (vertex[i][1].x - vertex[i][2].x);
            //cout << "I am OK 0.75" << endl;
            double y1 = (vertex[i][1].y- vertex[i][2].y);
            double x2 = (vertex[j][1].x- vertex[j][2].x);
            double y2 = (vertex[j][1].y- vertex[j][2].y);
            // cout << i << " " << j<<endl;
            // cout << x1 <<" " << y1 <<endl;
            // cout  << x2 << " " << y2  <<endl;
            if( abs((x1 * y2 - x2 * y1) / (y1 * y2)) < 0.013131313)//平行
            {
                // cout << "I am OK2" << endl;
                target = getMidpoint(vertex[i][0], vertex[i][1], vertex[j][3], vertex[j][2]);
                vector<Point2f> pnts; //
                
                vector<Point2f>vertex1; //
                vertex1.push_back(vertex[i][0]);vertex1.push_back(vertex[i][1]);vertex1.push_back(vertex[j][3]);vertex1.push_back(vertex[j][2]);
                for (int i = 0; i < 4; i++) //左上角第一个，顺时针排序
                {
                    if (vertex1[i].x < vertex1[4].x && vertex1[i].y < vertex1[4].y)
                        pnts.push_back(vertex1[i]);
                }
                for (int i = 0; i < 4; i++)
                {
                    if (vertex1[i].x > vertex1[4].x && vertex1[i].y < vertex1[4].y)
                        pnts.push_back(vertex1[i]);
                }
                for (int i = 0; i < 4; i++) 
                {
                    if (vertex1[i].x > vertex1[4].x && vertex1[i].y > vertex1[4].y)
                        pnts.push_back(vertex1[i]);
                }
                for (int i = 0; i < 4; i++) 
                {
                    if (vertex1[i].x < vertex1[4].x && vertex1[i].y > vertex1[4].y)
                        pnts.push_back(vertex1[i]);
                }                
                pnp(pnts, cameraMatrix, distCoeffs);
            }
            else continue;
        }
    }
    
    cout << "HELLO";
    return 0;
}


/*
https://blog.csdn.net/mailzst1/article/details/83141632?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522169979389516800225583129%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=169979389516800225583129&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-83141632-null-null.142^v96^pc_search_result_base7&utm_term=rect.points%E5%87%BD%E6%95%B0&spm=1018.2226.3001.4187
*/