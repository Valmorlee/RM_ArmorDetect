//
// Created by valmorx on 25-2-14.
//
#pragma once
#ifndef TOOL_HPP
#define TOOL_HPP
#include <ArmorDetector.hpp>

#endif //TOOL_HPP

#pragma once
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "LightDescriptor.hpp"
#include"ArmorDescriptor.hpp"
//调整倾斜角度
void adjustRec(cv::RotatedRect& rect);

//坐标两点间距离
double distance(const cv::Point &p1, const cv::Point &p2);

//筛选灯条
void filterContours(std::vector<std::vector<cv::Point>> &contours, std::vector<LightDescriptor> &lightInfos, ArmorDetector &detector);

//筛选装甲板
std::vector<ArmorDescriptor> filterArmors(std::vector<LightDescriptor> &lightInfos,ArmorDetector &detector);

//画装甲板：debug
void drawArmor(ArmorDetector &detector,cv::Mat &img);

//求两条直线的交点
cv::Point2f findIntersection(cv::Vec4f line1, cv::Vec4f line2);

//
void drawPoint(const cv::Mat &img,cv::Point2f pt);