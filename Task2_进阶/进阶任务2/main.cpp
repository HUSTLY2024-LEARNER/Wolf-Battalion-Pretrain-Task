#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat locateAndCorrectLicensePlate(Mat img_src, Mat img_mask, string name) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        cout << "未检测到车牌" << endl;
        return img_src;
    } else {
        int flag = 0; // 默认flag为0，因为不一定有车牌区域
        for (size_t ii = 0; ii < contours.size(); ii++) {
            Rect rect = boundingRect(contours[ii]);
            Mat img_cut_mask = img_mask(rect);
            if (mean(img_cut_mask)[0] >= 75 && rect.width > 15 && rect.height > 15) {
                RotatedRect rotatedRect = minAreaRect(contours[ii]);
                Point2f box[4];
                rotatedRect.points(box);

                vector<Point> cont;
                for (size_t i = 0; i < contours[ii].size(); i++) {
                    cont.push_back(contours[ii][i]);
                }

                // 由于转换矩阵的两组坐标位置需要一一对应，因此需要将最小外接矩形的坐标进行排序，最终排序为[左上，左下，右上，右下]
                sort(box, box + 4, [](Point2f a, Point2f b) {
                    return a.x < b.x;
                });
                Point2f box_left[2] = {box[0], box[1]};
                Point2f box_right[2] = {box[2], box[3]};
                sort(box_left, box_left + 2, [](Point2f a, Point2f b) {
                    return a.y < b.y;
                });
                sort(box_right, box_right + 2, [](Point2f a, Point2f b) {
                    return a.y < b.y;
                });

                Point2f final_box[4] = {box_left[0], box_left[1], box_right[0], box_right[1]};
                Point2f destination_points[4] = {Point2f(0, 0), Point2f(0, 80), Point2f(240, 0), Point2f(240, 80)};

                Mat transform_mat = getPerspectiveTransform(final_box, destination_points);
                Mat lic;
                warpPerspective(img_src, lic, transform_mat, Size(240, 80));

                if (contours.size() == 1) { // 只有一个区域可以认为是车牌区域
                    imshow("result", lic);
                    flag += 1;
                    cout << "saving " << name.substr(0, 7) << ".png" << endl;
                    imwrite(name.substr(0, 7) + ".png", lic);
                }
            }
        }

        if (!flag) {
            cout << "未检测到车牌区域或车牌区域过小" << endl;
        }

        return img_src;
    }
}
int main() 
{

    Mat image = imread("../img/车牌5.jpg");

    // BGR to HSV 
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // 筛选色域范围
    Scalar lower_blue = Scalar(100, 150, 100);  
    Scalar upper_blue = Scalar(160, 255, 255);  

    // 蓝色掩罩
    Mat blue_mask;
    inRange(hsv, lower_blue, upper_blue, blue_mask);
    imshow("blue_Mask", blue_mask);
    // 高斯滤波
    Mat blurred_mask;
    GaussianBlur(blue_mask, blurred_mask, Size(3, 3), 0);
    imshow("blurred_mask",blurred_mask);

    //腐蚀
    Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat eroded_mask;
    erode(blurred_mask, eroded_mask, kernel_erode);
    imshow("eroded_mask",eroded_mask);

    // 膨胀
    Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(30, 30));
    Mat dilated_mask;
    dilate(eroded_mask, dilated_mask, kernel_dilate);
    imshow("dilated_mask",dilated_mask);

    // 二值化
    Mat binary_mask;
    threshold(dilated_mask, binary_mask, 128, 255, THRESH_BINARY);
    imshow("Binary Mask", binary_mask);
    locateAndCorrectLicensePlate(image, binary_mask, "new");
    // findContours
    // vector<vector<Point>> contours;
    // findContours(binary_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Mat image_with_rectangles = image.clone();

    // for (const auto& contour : contours) //找出所有方框，事实上只有一个
    // {
    //     Rect bounding_rect = boundingRect(contour);
    //     rectangle(image_with_rectangles, bounding_rect, Scalar(0, 255, 0), 3);
    //     Point2f srcTriangle[3];
    //     srcTriangle[0] = bounding_rect.tl();
    //     srcTriangle[1] = bounding_rect.br();
    //     srcTriangle[2] = Point2f(bounding_rect.width + bounding_rect.x , bounding_rect.y);

    // }
    
    // imshow("RESULT", image_with_rectangles);
    waitKey(0);

    return 0;
}

//     Mat src = imread("../车牌3.png");
//     Mat dst = Mat::zeros(scr.size(), scr.type);
//     Mat M(2, 3, CV_32FC1);
//     M
//     warpAffine(src, dst, M, scr.size(), int flags=INTER_LINEAR, intborderMode=BORDER_CONSTANT, const Scalar& borderValue=Scalar())
// }
