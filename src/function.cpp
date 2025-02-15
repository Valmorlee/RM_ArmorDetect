//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"

#include <Parameter.hpp>

#include "tool.hpp"
#include "Center.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>

Parameter param;

cv::Mat func_colorDetect(const cv::Mat &img) {
    std::vector<cv::Mat> channels;
    cv::Mat output;
    cv::split(img,channels);

    if (param.enemy_color == 1) {
        output=channels.at(2)-channels.at(0);
    }else output=channels.at(0)-channels.at(2);

    cv::imshow("output",output);

    return output;
}

void func_armorDetect(cv::Mat &img) {
    cv::Mat img_origin=img.clone();
    img=func_colorDetect(img);

    cv::Mat img_gray,img_binary,img_result=img.clone();
    modifyHighlight(img);

    //灰度化
    // cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
    // cv::imshow("img_gray",img_gray);

    //二值化
    cv::threshold(img,img_binary,10,255,cv::THRESH_BINARY);
    cv::imshow("img_binary",img_binary);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3,3));
    cv::dilate(img_binary,img_binary,element);

    //寻找轮廓点
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img_binary,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    std::vector<LightDescriptor> lightInfos;
    std::vector<cv::RotatedRect> rotated_rects;
    for (const auto & contour :contours) {
        double lightContoursArea = cv::contourArea(contour);

        //面积小于阈值过滤
        if (lightContoursArea < param.light_min_area) continue;
        //椭圆拟合
        cv::RotatedRect rotated_rect = cv::fitEllipse(contour);
        //长宽比过大过滤
        if (rotated_rect.size.width / rotated_rect.size.height > param.light_max_ratio) continue;
        //轮廓面积与椭圆面积比小于阈值过滤
        if (lightContoursArea / rotated_rect.size.area() < param.light_contour_min_solidity) continue;

        rotated_rects.emplace_back(rotated_rect);
        lightInfos.emplace_back(LightDescriptor(rotated_rect));
    }

    if (lightInfos.empty()) {
        param.enemy_flag = 0;
        std::cout<<"未识别到装甲板"<<std::endl;
        return;
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
