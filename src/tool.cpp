//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include "LightDescriptor.hpp"
#include <Parameter.hpp>
#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define BIG_ARMOR 1
#define SMALL_ARMOR 0
#define UNKNOWN_ARMOR -1

Parameter param_tool;

void adjustRec(cv::RotatedRect& rect)
{
    float& width = rect.size.width;
    float& height = rect.size.height;
    float& angle = rect.angle;

    while(angle >= 90.0) angle -= 180.0;
    while(angle < -90.0) angle += 180.0;

    if(angle >= 45.0){
        std::swap(width, height);
        angle -= 90.0;
    }else if(angle < -45.0){
        std::swap(width, height);
        angle += 90.0;
    }

}

// 坐标两点间距离
double distance(const cv::Point2f &p1, const cv::Point2f &p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
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



void filterContours(std::vector<std::vector<cv::Point> > &contours, std::vector<LightDescriptor> &lightInfos) {
    for (const auto & contour :contours) {
        double lightContoursArea = cv::contourArea(contour);

        //面积小于阈值过滤
        if (lightContoursArea < param_tool.light_min_area) continue;
        //椭圆拟合
        cv::RotatedRect rotated_rect = cv::fitEllipse(contour);
        //角度修正
        adjustRec(rotated_rect);
        //长宽比过大过滤
        if (rotated_rect.size.width / rotated_rect.size.height > param_tool.light_max_ratio) continue;
        //轮廓面积与椭圆面积比小于阈值过滤
        if (lightContoursArea / rotated_rect.size.area() < param_tool.light_contour_min_solidity) continue;
        //扩大识别区域
        rotated_rect.size.width *= param_tool.light_area_extend_ratio;
        rotated_rect.size.height *= param_tool.light_area_extend_ratio;

        lightInfos.emplace_back(LightDescriptor(rotated_rect));
    }
}


std::vector<ArmorDescriptor> filterArmors(std::vector<LightDescriptor> &lightInfos) {
    std::vector<ArmorDescriptor> armors;

    std::sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor &a, const LightDescriptor &b) {
        return a.center.x < b.center.x;
    });

    for (int i=0;i<lightInfos.size();i++) {
        for (int j=i+1;j<lightInfos.size();j++) {
            const LightDescriptor &leftlight = lightInfos[i];
            const LightDescriptor &rightlight = lightInfos[j];

            //角差
            double angleDiff = abs(leftlight.angle - rightlight.angle);
            //长度差比率
            double LenDiff_Ratio = abs(leftlight.length - rightlight.length) / std::max(leftlight.length, rightlight.length);

            if (angleDiff < param_tool.light_max_angle_diff ||
                LenDiff_Ratio < param_tool.light_max_angle_diff_ratio) continue;

            //左右灯条中心点间距离
            double dis = distance(leftlight.center, rightlight.center);
            //左右灯条长度平均值
            double avgLen = (leftlight.length + rightlight.length) / 2;
            //左右灯条中心y差值
            double yDiff = abs(leftlight.center.y - rightlight.center.y);
            //左右灯条中心x差值
            double xDiff = abs(leftlight.center.x - rightlight.center.x);
            //y差值比率
            double yDiff_Ratio = yDiff / avgLen;
            //x差值比率
            double xDiff_Ratio = xDiff / avgLen;

            //相距距离与灯条长度比值
            double dis_ratio = dis / avgLen;

            //筛选符合条件的灯条
            if (yDiff_Ratio > param_tool.light_max_y_diff_ratio ||
                xDiff_Ratio > param_tool.light_min_x_diff_ratio ||
                dis_ratio > param_tool.armor_max_aspect_ratio||
                dis_ratio < param_tool.armor_min_aspect_ratio) continue;

            //确认装甲板类型
            int armor_flag = dis_ratio > param_tool.armor_type_big_ratio ? BIG_ARMOR : SMALL_ARMOR;

            //
        }
    }
    return {};
}
