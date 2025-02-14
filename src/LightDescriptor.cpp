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
}

LightDescriptor::LightDescriptor(std::vector<cv::Point2f> points) {

}

LightDescriptor::~LightDescriptor() {
    // 默认析构函数
}