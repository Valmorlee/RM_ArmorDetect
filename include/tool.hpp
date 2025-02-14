//
// Created by valmorx on 25-2-14.
//

#ifndef TOOL_HPP
#define TOOL_HPP

#endif //TOOL_HPP

#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>

//显示调试图像

//画点
void drawPoint(const cv::Mat &img, const std::vector<cv::Point> &points);
void drawPoint(const cv::Mat &img, const std::vector<cv::Point2f> &points);

//亮度&对比度（高光）调整
void modifyHighlight(const cv::Mat &img);
void modifyHighlight(const cv::Mat &img,double alpha, double beta);


