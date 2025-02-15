//
// Created by valmorx on 25-2-14.
//

#pragma once
#include "LightDescriptor.hpp"
#include "tool.hpp"
#include "function.hpp"
#include<bits/stdc++.h>

LightDescriptor::LightDescriptor() {
    // 默认构造函数
}

LightDescriptor::LightDescriptor(cv::RotatedRect rect) {
    this->center = rect.center;

    std::vector<cv::Point2f> pts;
    rect.points(pts);

    this->pts = pts;

    this->lu_x=pts[0].x; this->lu_y=pts[0].y;
    this->ru_x=pts[1].x; this->ru_y=pts[1].y;
    this->rd_x=pts[2].x; this->rd_y=pts[2].y;
    this->ld_x=pts[3].x; this->ld_y=pts[3].y;
    this->width = rect.size.width;
    this->length = rect.size.height;
    this->area = rect.size.area();
    this->angle = rect.angle;


}

// LightDescriptor::LightDescriptor(LightDescriptor &light) {
//     this->angle = light.angle;
//     this->lu_x=light.lu_x; this->ru_x=light.ru_x;
//     this->rd_x=light.rd_x; this->ld_x=light.ld_x;
//     this->lu_y=light.lu_y; this->ru_y=light.ru_y;
//     this->rd_y=light.rd_y; this->ld_y=light.ld_y;
//     this->center = light.center;
//     this->width = light.width;
//     this->length = light.length;
//     this->area = light.area;
//     this->pts = light.pts;
// }

LightDescriptor::~LightDescriptor() {
    // 默认析构函数
}

void LightDescriptor::printInfo() {
    std::cout<<"center: "<<this->center.x<<" "<<this->center.y<<std::endl;
    std::cout<<"[lu: "<<this->lu_x<<" "<<this->lu_y<<"]"<<std::endl;
    std::cout<<"[ru: "<<this->ru_x<<" "<<this->ru_y<<"]"<<std::endl;
    std::cout<<"[rd: "<<this->rd_x<<" "<<this->rd_y<<"]"<<std::endl;
    std::cout<<"[ld: "<<this->ld_x<<" "<<this->ld_y<<"]"<<std::endl;
    std::cout<<"angle: "<<this->angle<<std::endl;
}

void LightDescriptor::drawLight(cv::Mat &img) {
    cv::Point2f points[4] = {cv::Point2f(this->lu_x, this->lu_y),
                            cv::Point2f(this->ru_x, this->ru_y),
                            cv::Point2f(this->rd_x, this->rd_y),
                            cv::Point2f(this->ld_x, this->ld_y)};
    cv::line(img, points[0], points[1], cv::Scalar(0, 0, 255), 2);
    cv::line(img, points[1], points[2], cv::Scalar(0, 0, 255), 2);
    cv::line(img, points[2], points[3], cv::Scalar(0, 0, 255), 2);
    cv::line(img, points[3], points[0], cv::Scalar(0, 0, 255), 2);

}
