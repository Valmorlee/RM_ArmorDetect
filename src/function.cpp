//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"

#include <Parameter.hpp>

#include "tool.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>

Parameter param;

cv::Mat func_colorDetect(const cv::Mat &img) {
    std::vector<cv::Mat> channels;
    cv::Mat output;
    cv::split(img,channels);

    if (param.enemy_color == 1) {
        output=channels.at(2)-channels.at(0);
    }else output=channels.at(0)-channels.at(2);

    //cv::imshow("output",output);
    return output;
}

void func_armorDetect(const cv::Mat &img) {
    cv::Mat img_origin=img.clone();
    cv::Mat img_gray,img_binary,img_result=img.clone();

    img_gray=func_colorDetect(img);

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

    filterContours(contours,lightInfos);
    //debug输出
    for (LightDescriptor x:lightInfos) {
        x.printInfo();
        x.drawLight(img_origin);
    }

    if (lightInfos.empty()) {
        param.enemy_flag = 0;
        std::cout<<"未识别到装甲板"<<std::endl;
        return;
    }

    cv::imshow("img_origin",img_origin);

    cv::waitKey(0);

}
