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
    timeval t1, t2;
    if (detector.is_timeMonitor) {
        gettimeofday(&t1, NULL);
    }

    //cv::Mat img_origin=img.clone();
    cv::Mat img_gray,img_binary,img_result=img.clone();

    img_gray=func_colorDetect(img,detector);
    detector._grayImg=img_gray;

    //灰度化 （通道相减已无需灰化）
    //cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
    //cv::imshow("img_gray",img_gray);

    //二值化
    cv::threshold(img_gray,img_binary,detector.param.threshold_index,255,cv::THRESH_BINARY);

    if (detector.is_debug) {
        cv::imshow("img_gray",img_gray);
        cv::imshow("img_binary",img_binary);
    }
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,detector.param.light_dilate_size);
    cv::dilate(img_binary,img_binary,element);

    //寻找轮廓点
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img_binary,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    std::vector<LightDescriptor> lightInfos;

    filterContours(contours,lightInfos,detector);

    //debug输出
    for (LightDescriptor x:lightInfos) {
        //x.printInfo(); //打印二维四点坐标以及角度和中心
        if (detector.is_debug) {
            x.drawLight(detector._displayImg);
        }
    }

    if (lightInfos.empty()) {
        param.enemy_flag = 0;
        if (detector.is_debug) std::cout<<"未识别到装甲板"<<std::endl;
        return;
    }

    if (detector.is_debug) { //debug开关
        cv::imshow("img_origin",detector._displayImg);
    }

    //匹配灯条对
    detector._armors=filterArmors(lightInfos,detector);
    if (detector._armors.empty()) {
        if (detector.is_debug) std::cout << "armors empty!" << std::endl;
        return;
    }else {
        func_KalmanPre(detector);
    }

    drawArmor(detector,detector._displayImg);

    if (detector.is_timeMonitor) {
        gettimeofday(&t2, NULL);
        double deltaT = (t2.tv_sec-t1.tv_sec) * 1000000+ (t2.tv_usec-t1.tv_usec) ;

        std::cout << "Time: " << deltaT / 1000.0 << " ms"<< std::endl;
    }

    for (int i=0;i<detector._armors.size();i++) {
        drawPoint(detector._displayImg,detector._armors[i].centerPoint,cv::Scalar(0,0,255));
    }


}

void func_KalmanPre(ArmorDetector &detector) {
        for (int i=0;i<detector._armors.size();i++) {

            ArmorDescriptor& armor = detector._armors[i];
            detector.measurement.at<float>(0) = armor.centerPoint.x;
            detector.measurement.at<float>(1) = armor.centerPoint.y;

            // 更新卡尔曼滤波器
            detector.kf.correct(detector.measurement);

            // 预测
            detector.state = detector.kf.predict();
            float predict_x = detector.state.at<float>(0);
            float predict_y = detector.state.at<float>(1);

            // 合成Point_pre
            cv::Point2f Point_pre = cv::Point2f(predict_x, predict_y);
            detector._armors[i].Point_pre = Point_pre;
            drawPoint(detector._displayImg,Point_pre,cv::Scalar(255,0,255));
        }
}

void func_YoloDetect(const cv::Mat &img,ArmorDetector &detector) {

    cv::Size dstSize = cv::Size(detector.param.onnx_width,detector.param.onnx_height);
    object_rect effect_roi;
    cv::Mat resized_img = img.clone();

    yolo_resizeUniform(img,resized_img,dstSize,effect_roi,detector);

    auto results = yolo_PreDetect(resized_img,detector);

    cv::Mat final = yolo_draw_bboxes(img,results,effect_roi,detector);

}
