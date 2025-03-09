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

//画点
void drawPoint(const cv::Mat &img,cv::Point2f pt,const cv::Scalar &color);

//yolo 图像尺寸比例预处理
void yolo_resizeUniform(const cv::Mat &src,cv::Mat &resized_img,cv::Size dstSize,object_rect &effect_roi,ArmorDetector &detector);

//yolo 图像皈依化
void yolo_normalize(cv::Mat &img, ArmorDetector &detector);

//yolo 生成网格中心点
void yolo_getGridCenterX(std::vector<CenterX> &centers, ArmorDetector &detector);

inline float fast_exp(float x);

int yolo_activation_function_softmax(const float *src, float *dst, int length);

Info yolo_recPred2Box(const float* BoundBoxPre,int label_max_conf,float conf,const int x,const int y,const int stride,ArmorDetector &detector);

void yolo_decode2Box(cv::Mat &out,std::vector<CenterX> &centers,double threshold,std::vector<std::vector<Info>> &res_infos,ArmorDetector &detector);

//yolo 预测前处理
std::vector<Info> yolo_PreDetect(cv::Mat &resized_img,ArmorDetector &detector);

//yolo 预测后处理
std::vector<Info> yolo_PostDetect(cv::Mat &out, cv::Mat &resized_img, ArmorDetector &detector);

cv::Mat yolo_draw_bboxes(const cv::Mat &bgr, std::vector<Info> &bboxes, object_rect &effect_roi,ArmorDetector &detector);

