//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>

// 显示调试图片
void ims(const cv::Mat img,std::string name) {
    if (img.empty()) {
        std::cout<<"ims: img empty!"<<std::endl;
        return;
    }

    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::imshow(name, img);
}

// 绘制轮廓点
void drawPoint(const cv::Mat &img, const std::vector<cv::Point> &points)
{
    for(cv::Point point:points){
        cv::circle(img, point, 1, cv::Scalar(0, 0, 255), 3);
    }
}

void drawPoint(const cv::Mat &img, const std::vector<cv::Point2f> &points)
{
    for(cv::Point2f point:points){
        cv::circle(img, point, 1, cv::Scalar(0, 0, 255), 3);
    }
}

// 修改亮度&对比度
void modifyHighlight(const cv::Mat &img) {
    modifyHighlight(img, 1.5, -200);
}

void modifyHighlight(const cv::Mat &img,double alpha, double beta) {
    img.convertTo(img, -1, alpha, beta);
}

