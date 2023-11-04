/*https://blog.csdn.net/zhuoqingjoking97298/article/details/122259409?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522169875756616777224440090%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=169875756616777224440090&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-2-122259409-null-null.142^v96^pc_search_result_base4&utm_term=%E6%AC%A7%E6%8B%89%E8%A7%92%E8%BD%AC%E6%97%8B%E8%BD%AC%E7%9F%A9%E9%98%B5&spm=1018.2226.3001.4187*/
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>
using namespace std;
using namespace cv;

Mat euler_to_rotMat(double yaw, double pitch, double roll) {
    Mat Rz_yaw = (Mat_<double>(3, 3) <<
        cos(yaw), -sin(yaw), 0,
        sin(yaw), cos(yaw), 0,
        0, 0, 1);
    Mat Ry_pitch = (Mat_<double>(3, 3) <<
        cos(pitch), 0, sin(pitch),
        0, 1, 0,
        -sin(pitch), 0, cos(pitch));
    Mat Rx_roll = (Mat_<double>(3, 3) <<
        1, 0, 0,
        0, cos(roll), -sin(roll),
        0, sin(roll), cos(roll));
    Mat rotMat = Rz_yaw * (Ry_pitch * Rx_roll);
    return rotMat;
}

void visualizeRotation(double yaw, double pitch, double roll) {
    // Create a 3D coordinate system
    viz::Viz3d myWindow("Coordinate System Visualization");

    viz::WCoordinateSystem world_coor(1.0);
    myWindow.showWidget("World", world_coor);

    Mat Rmat = euler_to_rotMat(yaw, pitch, roll);

    Affine3d pose(Rmat);

    
    viz::WCoordinateSystem rotated_coor(0.5);
    myWindow.showWidget("Rotated Coordinate System", rotated_coor);
    myWindow.setWidgetPose("Rotated Coordinate System", pose);

    myWindow.spin();
}

int main() {
   
    double yaw, pitch, roll;
    cout << "Enter yaw angle (in radians): ";
    cin >> yaw;
    cout << "Enter pitch angle (in radians): ";
    cin >> pitch;
    cout << "Enter roll angle (in radians): ";
    cin >> roll;

    // Visualize the rotation
    visualizeRotation(yaw, pitch, roll);

    return 0;
}

