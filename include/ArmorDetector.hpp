//
// Created by valmorx on 25-2-16.
//
#pragma once

#ifndef ARMORDETECTOR_HPP
#define ARMORDETECTOR_HPP

#include <Parameter.hpp>
#include <opencv2/opencv.hpp>
#include"ArmorDescriptor.hpp"
#include<eigen3/Eigen/Dense>


#endif //ARMORDETECTOR_HPP

#define BLUE 0
#define RED 1

class object_rect {
public:

    object_rect() {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    ~object_rect() = default;

    int x;
    int y;
    int width;
    int height;

};

class Info {
public:
    Info() {
        x1 = 0;
        y1 = 0;
        x2 = 0;
        y2 = 0;
        conf = 0;
        label = -1;
    }

    Info(float x1, float y1, float x2, float y2, float conf, int label) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
        this->conf = conf;
        this->label = label;
    }

    ~Info() = default;

    float x1;
    float y1;
    float x2;
    float y2;

    float conf;
    int label;
};

class CenterX {
public:

    int x;
    int y;
    int stride; //卷积采样间隔

};


class ArmorDetector {
public:

    bool is_timeMonitor =
        true
    ; //帧延迟监控开关

    bool is_debug =
        false
    ; //调试开关

    ArmorDetector() {
        //init Kalman
        kf = cv::KalmanFilter(4, 2, 0);
        kf.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0,
                                                            0, 1, 0, 1,
                                                            0, 0, 1, 0,
                                                            0, 0, 0, 1);
        kf.measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0,
                                                              0, 1, 0, 0);
        kf.processNoiseCov = (cv::Mat_<float>(4, 4) << 1e-2, 0, 0, 0,
                                                            0, 1e-2, 0, 0,
                                                            0, 0, 1, 0,
                                                            0, 0, 0, 1) * 1e-2;
        kf.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1e-1, 0,
                                                            0, 1e-1) * 1e-1;
        kf.errorCovPost = cv::Mat::eye(4, 4, CV_32F);
        state = cv::Mat::zeros(4, 1, CV_32F);
        measurement = cv::Mat::zeros(2, 1, CV_32F);

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

        this->YOLO_Center_Points.clear();

        _srcImg = img;
        _displayImg = img.clone();

        cv::Rect imgBound = cv::Rect(cv::Point(50,50),cv::Point(_srcImg.cols-50,_srcImg.rows-50));

        _roi=imgBound;
        _roiImg = _srcImg(_roi).clone(); //原点坐标会移动到裁剪的左上角


    }

    void detect() {

    }

    Parameter param;
    std::vector<ArmorDescriptor> _armors;

    cv::KalmanFilter kf;
    cv::Mat state; // (x, y, vx, vy)
    cv::Mat measurement; // (x, y)

    int _enemy_color;
    int _self_color;

    cv::Rect _roi;

    cv::Mat _srcImg;
    cv::Mat _roiImg;
    cv::Mat _grayImg;

    cv::Mat _displayImg;

    cv::dnn::Net myNet;
    std::vector<cv::Point> YOLO_Center_Points;

};

