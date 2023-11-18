#include "Simulator.hpp"
#include "LowPassFilter.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main() {
    // 创建运动模型的模拟器
    Simulator<double, 2> *simulator;
    simulator = new Simulator<double, 2>(Eigen::Vector2d(5, 5), 5, Eigen::Vector2d(0.3, 0.2)); // 输入为起始点与方差

    // 定义低通滤波器的采样率和截止频率
    double sample_rate = 100.0;
    double cutoff_frequency = 5.0;

    // 创建低通滤波器
    LowPassFilter<double, 2> filter(sample_rate, cutoff_frequency);
    cv::Mat img(1000, 1000, CV_8UC3, cv::Scalar(0, 0, 0));
    // 模拟运动并进行滤波
    for(int i = 0; i < 1000 ; i ++) {
        // 获取测量值
        Eigen::Matrix<double, 2, 1> measurement = simulator->getMeasurement(i);

        // 使用低通滤波器进行滤波
        Eigen::Matrix<double, 2, 1> estimate = filter.update(measurement);

        // 输出结果
        cv::circle(img, cv::Point((int)(measurement[0] * 10 + 20), int(measurement[1] * 10 + 20)), 2, cv::Scalar(0, 0, 255), -1);
        cv::circle(img, cv::Point((int)(estimate[0] * 10 + 20), (int)(estimate[1] * 10 + 20)), 2, cv::Scalar(0, 255, 0), -1);

        cv::imshow("img", img);
        cv::waitKey(10);
    }

    return 0;
}
