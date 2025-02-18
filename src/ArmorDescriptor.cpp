//
// Created by valmorx on 25-2-15.
//

#include "ArmorDescriptor.hpp"

#include <Parameter.hpp>
#include <tool.hpp>
#include<bits/stdc++.h>

#define BIG_ARMOR 1
#define SMALL_ARMOR 0
#define UNKNOWN_ARMOR -1

ArmorDescriptor::ArmorDescriptor() {
    this->distScore = 0;
    this->finalScore = 0;
    this->rotationScore = 0;
    this->sizeScore = 0;
    this->centerPoint = cv::Point2f(0,0);

    this->armortype = UNKNOWN_ARMOR;
    vertex.resize(4);

    for (int i=0;i<4;i++) {
        vertex[i] = cv::Point2f(0,0);
    }


}

void ArmorDescriptor::getFrontImg(const cv::Mat &grayImg) {
    const cv::Point2f& lu = vertex[0];
    const cv::Point2f& ru = vertex[1];
    const cv::Point2f& rl = vertex[2];
    const cv::Point2f& ll = vertex[3];

    int width,height;
    if (armortype == BIG_ARMOR) {
        width = 92; height = 50;
    }else {
        width = 50; height = 50;
    }

    //透视变换
    cv::Point2f src[4]{cv::Vec2f(lu),cv::Vec2f(ru),cv::Vec2f(rl),cv::Vec2f(ll)};
    cv::Point2f dst[4]{cv::Vec2f(0,0),cv::Vec2f(width,0),cv::Vec2f(width,height),cv::Vec2f(0,height)};
    const cv::Mat persMat = cv::getPerspectiveTransform(src,dst);

    cv::warpPerspective(grayImg,frontImg,persMat,cv::Size(width,height));

}

ArmorDescriptor::ArmorDescriptor(const LightDescriptor &leftLight,const LightDescriptor &rightLight,int armorType,cv::Mat roi_gray_img,double rotationScore,Parameter param) {
    this->lightPairs[0] = leftLight.rec();
    this->lightPairs[1] = rightLight.rec();
    this->armortype = armorType;
    this->rotationScore = rotationScore;

    //灯条高度扩大两倍画旋转矩形
    cv::Size leftSize(int (lightPairs[0].size.width),int (lightPairs[0].size.height * 2));
    cv::Size rightSize(int (lightPairs[1].size.width),int (lightPairs[1].size.height * 2));
    cv::RotatedRect armor_leftLight(lightPairs[0].center,leftSize,lightPairs[0].angle);
    cv::RotatedRect armor_rightLight(lightPairs[1].center,rightSize,lightPairs[1].angle);

    std::vector<cv::Point2f> leftPoints(4);
    std::vector<cv::Point2f> rightPoints;

    armor_leftLight.points(leftPoints);
    armor_rightLight.points(rightPoints);

    cv::Point2f point_lu = leftPoints[2]; //左上点
    cv::Point2f point_ll = leftPoints[3]; //左下点
    cv::Point2f point_ru = rightPoints[1]; //右上点
    cv::Point2f point_rl = rightPoints[0]; //右下点

    vertex.resize(4); //实参设置四个顶点
    vertex[0] = point_lu; vertex[1] = point_ru;
    vertex[2] = point_rl; vertex[3] = point_ll;

    double normalized_area = cv::contourArea(vertex) / param.area_normalized_base;
    sizeScore = exp(normalized_area);

    getFrontImg(roi_gray_img); //等所有完成再确认

    //x形
    cv::Vec4f line1(vertex[0].x,vertex[0].y,vertex[2].x,vertex[2].y);
    cv::Vec4f line2(vertex[1].x,vertex[1].y,vertex[3].x,vertex[3].y);

    centerPoint = findIntersection(line1,line2);

}

void ArmorDescriptor::clear() {

}

ArmorDescriptor::~ArmorDescriptor() {

}