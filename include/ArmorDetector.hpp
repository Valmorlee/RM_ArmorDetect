//
// Created by valmorx on 25-2-16.
//
#pragma once

#ifndef ARMORDETECTOR_HPP
#define ARMORDETECTOR_HPP

#include <Parameter.hpp>
#include <opencv2/opencv.hpp>
#include"ArmorDescriptor.hpp"


#endif //ARMORDETECTOR_HPP


class ArmorDetector {
public:

    void init() {

    }

    void loadImg() {

    }

    void detect() {

    }

    Parameter param;
    std::vector<ArmorDescriptor> _armors;

    int _enemy_color;
    int _self_color;

    cv::Rect _roi;

    cv::Mat _srcImg;
    cv::Mat _roiImg;
    cv::Mat _grayImg;



};
