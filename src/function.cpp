//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"

#include <Parameter.hpp>

#include "tool.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>
#include <sys/time.h>

Parameter param;

#define BLUE 0
#define RED 1

cv::Mat func_colorDetect(const cv::Mat &img,ArmorDetector &detector) {
    std::vector<cv::Mat> channels;
    cv::Mat output;
    cv::split(img,channels);

    if (detector._enemy_color == RED) {
        output=channels.at(2)-channels.at(0);
    }else output=channels.at(0)-channels.at(2);

    //cv::imshow("output",output);
    return output;
}

void func_armorDetect(const cv::Mat &img, ArmorDetector &detector) {
    struct timeval t1, t2;
    if (detector.is_timeMonitor) {
        gettimeofday(&t1, NULL);
    }

    cv::Mat img_origin=img.clone();
    cv::Mat img_gray,img_binary,img_result=img.clone();

    img_gray=func_colorDetect(img,detector);
    detector._grayImg=img_gray;

    //灰度化 （通道相减已无需灰化）
    //cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
    //cv::imshow("img_gray",img_gray);

    //二值化
    cv::threshold(img_gray,img_binary,120,255,cv::THRESH_BINARY);
    cv::imshow("img_binary",img_binary);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3,3));
    cv::dilate(img_binary,img_binary,element);

    //寻找轮廓点
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img_binary,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    std::vector<LightDescriptor> lightInfos;

    filterContours(contours,lightInfos,detector);

    //debug输出
    for (LightDescriptor x:lightInfos) {
        //x.printInfo(); //打印二维四点坐标以及角度和中心
        x.drawLight(img_origin);
    }

    if (lightInfos.empty()) {
        param.enemy_flag = 0;
        std::cout<<"未识别到装甲板"<<std::endl;
        return;
    }

    cv::imshow("img_origin",img_origin);

    //匹配灯条对
    detector._armors=filterArmors(lightInfos,detector);
    if (detector._armors.empty()) {
        std::cout << "armors empty!" << std::endl;
        return;
    }

    drawArmor(detector,img_origin);

    if (detector.is_timeMonitor) {
        gettimeofday(&t2, NULL);
        double deltaT = (t2.tv_sec-t1.tv_sec) * 1000000+ (t2.tv_usec-t1.tv_usec) ;

        std::cout << "Time: " << deltaT / 1000.0 << " ms"<< std::endl;
    }

}
