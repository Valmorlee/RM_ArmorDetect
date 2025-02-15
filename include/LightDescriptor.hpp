//
// Created by valmorx on 25-2-14.
//

#ifndef LIGHTDESCRIPTOR_HPP
#define LIGHTDESCRIPTOR_HPP

#endif //LIGHTDESCRIPTOR_HPP

#pragma once
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

class LightDescriptor{

public:
    LightDescriptor();
    //LightDescriptor(LightDescriptor &light);
    LightDescriptor(cv::RotatedRect rect);
    ~LightDescriptor();

    void printInfo();
    void drawLight(cv::Mat &img);


    //int color;
    double lu_x,lu_y,ru_x,ru_y; // 左上x，左上y，右上x，右上y
    double rd_x,rd_y,ld_x,ld_y; // 右下x，右下y，左下x，左下y
    std::vector<cv::Point2f> pts;
    cv::Point2f center;

    double width;
    double length;
    double area;
    double angle;

};