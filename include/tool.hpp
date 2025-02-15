//
// Created by valmorx on 25-2-14.
//

#ifndef TOOL_HPP
#define TOOL_HPP

#endif //TOOL_HPP

#pragma once
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "LightDescriptor.hpp"
#include"ArmorDescriptor.hpp"

//坐标两点间距离
double distance(const cv::Point &p1, const cv::Point &p2);

//画点
void drawPoint(const cv::Mat &img, const std::vector<cv::Point> &points);
void drawPoint(const cv::Mat &img, const std::vector<cv::Point2f> &points);

//亮度&对比度（高光）调整
void modifyHighlight(const cv::Mat &img);
void modifyHighlight(const cv::Mat &img,double alpha, double beta);

//筛选灯条
void filterContours(std::vector<std::vector<cv::Point>> &contours, std::vector<LightDescriptor> &lightInfos);

//筛选装甲板
std::vector<ArmorDescriptor> filterArmors(std::vector<LightDescriptor> &lightInfos);