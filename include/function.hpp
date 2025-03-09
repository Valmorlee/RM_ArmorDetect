//
// Created by valmorx on 25-2-14.
//
#pragma once
#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include <ArmorDetector.hpp>

#endif //FUNCTION_HPP

#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>

//颜色识别
cv::Mat func_colorDetect(const cv::Mat &img,ArmorDetector &detector);

void func_armorDetect(const cv::Mat &img,ArmorDetector &detector);

void func_KalmanPre(ArmorDetector &detector);

void func_YoloDetect(const cv::Mat &img,ArmorDetector &detector);