//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "ArmorDetector.hpp"

void PhotoOption() {
    cv::Mat img = cv::imread("../PhotoLib/Armor03.jpg"); // 读取图像
    cv::imshow("img", img);

    if (img.empty()) {
        std::cout << "Empty" << std::endl;
        return ;
    }

    ArmorDetector detector;
    detector.init(RED);
    detector.loadImg(img);

    func_armorDetect(img,detector);
    // cv::Mat imgx = detector._armors[0].frontImg;
    // cv::resize(imgx,imgx,cv::Size(400,400));
    //
    // cv::imshow("imgx", imgx);

}

void VideoOption() {
    ArmorDetector detector;
    detector.init(RED);
    cv::VideoCapture cap("/home/valmorx/BaiduDiskDownload/ArmorVideo01.MOV");
    cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));

    cv::Mat input;
    while (true) {
        cap.read(input);
        if (input.empty()) {
            std::cout << "inputEmpty" << std::endl;
            return ;
        }

        detector.loadImg(input);
        func_armorDetect(input,detector);

        cv::imshow("img", detector._displayImg);

        char c=cv::waitKey(30);
        if (c==27) break;
    }
    cap.release();
}

void YoloOption() {
    ArmorDetector detector;
    detector.init(RED);

    cv::VideoCapture cap("/home/valmorx/下载/video.mp4");
    cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));

    std::string model_path = detector.param.onnx_path;
    int model_w =  detector.param.onnx_width;
    int model_h =  detector.param.onnx_height;

    cv::dnn::Net mynet = cv::dnn::readNetFromONNX(model_path);
    detector.myNet = mynet;

    if (mynet.empty()) {
        std::cout<<"Empty ONNX Net"<<std::endl;
        return;
    }

    cv::Mat input;
    while (true) {
        cap.read(input);
        if (input.empty()) {
            std::cout<<"input Empty"<<std::endl;
            return;
        }

        detector.loadImg(input);
        func_YoloDetect(input,detector);

        cv::imshow("img", detector._displayImg);


        cv::waitKey(0);

        char c=cv::waitKey(30);
        if (c==27) break;


    }
    cap.release();
}

int main() {
    //VideoOption();
    //PhotoOption();

    YoloOption();
    cv::waitKey(0);
}