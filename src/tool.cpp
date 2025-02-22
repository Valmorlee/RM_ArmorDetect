//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include <ArmorDetector.hpp>
#include "LightDescriptor.hpp"
#include <Parameter.hpp>
#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define BIG_ARMOR 1
#define SMALL_ARMOR 0
#define UNKNOWN_ARMOR -1

//Parameter param_tool;

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


void filterContours(std::vector<std::vector<cv::Point> > &contours, std::vector<LightDescriptor> &lightInfos, ArmorDetector &detector) {
    for (const auto & contour :contours) {
        double lightContoursArea = cv::contourArea(contour);

        //面积小于阈值过滤
        if (lightContoursArea < detector.param.light_min_area) continue;
        //椭圆拟合
        cv::RotatedRect rotated_rect = cv::fitEllipse(contour);
        //角度修正
        adjustRec(rotated_rect);
        //长宽比过大过滤
        if (rotated_rect.size.width / rotated_rect.size.height > detector.param.light_max_ratio) continue;
        //轮廓面积与椭圆面积比小于阈值过滤
        if (lightContoursArea / rotated_rect.size.area() < detector.param.light_contour_min_solidity) continue;
        //扩大识别区域
        rotated_rect.size.width *= detector.param.light_area_extend_ratio;
        rotated_rect.size.height *= detector.param.light_area_extend_ratio;

        lightInfos.emplace_back(LightDescriptor(rotated_rect));
    }
}


std::vector<ArmorDescriptor> filterArmors(std::vector<LightDescriptor> &lightInfos, ArmorDetector &detector) {
    std::vector<ArmorDescriptor> armors;

    std::sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor &a, const LightDescriptor &b) {
        return a.center.x < b.center.x;
    });

    int pairs1 = 0,pairs2 = 0,pairs3 = 0;

    for (int i=0;i<lightInfos.size();i++) {
        for (int j=i+1;j<lightInfos.size();j++) {
            const LightDescriptor &leftlight = lightInfos[i];
            const LightDescriptor &rightlight = lightInfos[j];


            pairs1++;

            //角差
            double angleDiff = abs(leftlight.angle - rightlight.angle);
            //长度差比率
            double LenDiff_Ratio = abs(leftlight.length - rightlight.length) / std::max(leftlight.length, rightlight.length);

            if (angleDiff > detector.param.light_max_angle_diff ||
                LenDiff_Ratio > detector.param.light_max_angle_diff_ratio) continue;

            pairs2++;

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
             if (yDiff_Ratio > detector.param.light_max_y_diff_ratio ||
                 xDiff_Ratio < detector.param.light_min_x_diff_ratio ||
                 dis_ratio > detector.param.armor_max_aspect_ratio||
                 dis_ratio < detector.param.armor_min_aspect_ratio) {

                 continue;
             }

            pairs3++;

            //确认装甲板类型
            int armor_flag = dis_ratio > detector.param.armor_type_big_ratio ? BIG_ARMOR : SMALL_ARMOR;

            //Score
            double rotationScore = 100;

            // cv::Mat img = cv::Mat::ones(cv::Size(200,200),CV_8UC3);
            // cv::circle(img,leftlight.center,2,cv::Scalar(0,0,255),-1);
            // cv::circle(img,rightlight.center,2,cv::Scalar(0,0,255),-1);
            // cv::imshow("debug",img);
            // cv::waitKey(0);

            ArmorDescriptor armor(leftlight, rightlight,armor_flag,detector._grayImg,rotationScore,detector.param);

            armors.emplace_back(armor);
            break;
        }
    }

    /*
    std::cout << "初始遴选个数： " << pairs1 << std::endl
              << "条件2遴选后个数： " << pairs2 << std::endl
              << "条件3遴选后个数： " << pairs3 << std::endl;

    */
    return armors;
}

void drawArmor(ArmorDetector &detector,cv::Mat &img) {
    for (int i=0;i<detector._armors.size();i++) {
        std::vector<cv::Point2i> points;
        for (int j=0;j<4;j++) {
            points.emplace_back(cv::Point(static_cast<int>(detector._armors[i].vertex[j].x),
                                          static_cast<int>(detector._armors[i].vertex[j].y)));

        }

        cv::polylines(img, points, true, cv::Scalar(0, 255, 0), 2,8,0);
    }
    if (detector.is_debug) {
        cv::imshow("armors", img);
    }
}

cv::Point2f findIntersection(cv::Vec4f line1, cv::Vec4f line2) {
    // 提取直线的端点
    float x1 = line1[0], y1 = line1[1];
    float x2 = line1[2], y2 = line1[3];
    float x3 = line2[0], y3 = line2[1];
    float x4 = line2[2], y4 = line2[3];

    // 计算行列式的值
    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // 如果分母为0，说明直线平行或重合，没有唯一交点
    if (denominator == 0) {
        return cv::Point2f(-1, -1); // 返回一个无效点表示没有交点
    }

    // 计算交点坐标
    float px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
    float py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

    return cv::Point2f(px, py);
}

void drawPoint(const cv::Mat &img, cv::Point2f pt,const cv::Scalar &color) {
    cv::circle(img, pt, 5, color, -1);
}

