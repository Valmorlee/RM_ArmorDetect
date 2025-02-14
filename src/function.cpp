//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"
#include "tool.hpp"
#include "Center.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>

double light_min_area = 1000;

int func_colorDetect(const cv::Mat &img, cv::Mat &mask, int color) {
    return 1;
}

void func_armorDetect(const cv::Mat &img) {
    cv::Mat img_origin=img.clone();
    cv::Mat img_gray,img_binary,img_result=img.clone();
    modifyHighlight(img);

    //灰度化
    cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
    cv::imshow("img_gray",img_gray);

    //二值化
    cv::threshold(img_gray,img_binary,120,255,cv::THRESH_BINARY);
    cv::imshow("img_binary",img_binary);

    //寻找轮廓点
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img_binary,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::RotatedRect> rotated_rects;
    for (const auto & contour :contours) {
        auto rotated_rect = cv::minAreaRect(contour);
        //面积过小过滤
        if (rotated_rect.size.area() < light_min_area) continue;
        rotated_rects.emplace_back(rotated_rect);
    }

    cv::Mat drawRect = img_origin.clone();
    for (const auto & rotated_rect : rotated_rects) {
        std::vector<cv::Point2f> points(4);
        rotated_rect.points(points);

        for (auto & point : points) {
            std::cout << point << std::endl;
        }

        drawPoint(drawRect,points);
    }

    cv::imshow("drawRect",drawRect);

    cv::waitKey(0);

}
