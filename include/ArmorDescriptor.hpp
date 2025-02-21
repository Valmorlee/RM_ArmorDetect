//
// Created by valmorx on 25-2-15.
//
#pragma once
#ifndef ARMORDESCRIPTOR_HPP
#define ARMORDESCRIPTOR_HPP
#include <LightDescriptor.hpp>


#endif //ARMORDESCRIPTOR_HPP


class Parameter;
class LightDescriptor;

class ArmorDescriptor {
public:
    ArmorDescriptor();

    ArmorDescriptor(const LightDescriptor &leftLight,const LightDescriptor &rightLight,int armorType,cv::Mat roi_gray_img,double rotationScore,Parameter param);
    ~ArmorDescriptor();

    void clear();
    void getFrontImg(const cv::Mat& grayImg);

    std::array<cv::RotatedRect, 2> lightPairs; //0 left, 1 right
    float sizeScore;		//Score1 = e^(size)
    float distScore;		//Score2 = e^(-offset)
    float rotationScore;		//Score3 = -(ratio^2 + yDiff^2)
    float finalScore;

    std::vector<cv::Point2f> vertex; //装甲板的四个顶点
    cv::Mat frontImg; //单通道的装甲板图案

    cv::Point2f centerPoint;
    cv::Point2f Point_pre;//下帧预测点

    int armortype;

    /*
     *1 -> big
     *0 -> small
     *-1 -> unknown
     */

};
