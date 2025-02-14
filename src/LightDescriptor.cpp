//
// Created by valmorx on 25-2-14.
//


#include "LightDescriptor.hpp"
#include "tool.hpp"
#include "function.hpp"
#include<bits/stdc++.h>

LightDescriptor::LightDescriptor() {
    // 默认构造函数
}

LightDescriptor::LightDescriptor(cv::RotatedRect rect) {
    this->center.set_x(rect.center.x);
    this->center.set_y(rect.center.y);

    std::vector<cv::Point2f> pts;
    rect.points(pts);

    this->lu_x=pts[0].x; this->lu_y=pts[0].y;
    this->ru_x=pts[1].x; this->ru_y=pts[1].y;
    this->rd_x=pts[2].x; this->rd_y=pts[2].y;
    this->ld_x=pts[3].x; this->ld_y=pts[3].y;


}

LightDescriptor::LightDescriptor(std::vector<cv::Point2f> points) {

}

LightDescriptor::~LightDescriptor() {
    // 默认析构函数
}