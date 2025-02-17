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

#define BLUE 0
#define RED 1

class ArmorDetector {
public:

    bool is_timeMonitor = true ; //帧延迟监控开关

    ArmorDetector() {

    }

    void init(int color_self) {
        if (color_self == RED) {
            _enemy_color = BLUE;
            _self_color = RED;
        }else {
            _enemy_color = RED;
            _self_color = BLUE;
        }

        std::cout<<"machine init complete!"<<std::endl;
    }

    void loadImg(cv::Mat& img) {
        _srcImg = img;

        cv::Rect imgBound = cv::Rect(cv::Point(50,50),cv::Point(_srcImg.cols-50,_srcImg.rows-50));

        _roi=imgBound;
        _roiImg = _srcImg(_roi).clone(); //原点坐标会移动到裁剪的左上角

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

