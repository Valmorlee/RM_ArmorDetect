//
// Created by valmorx on 25-2-14.
//

#include "function.hpp"
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "ArmorDetector.hpp"

void PhotoOption(const std::string &media_path) {
    cv::Mat img = cv::imread(media_path); // 读取图像
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

void VideoOption(const std::string &media_path) {
    ArmorDetector detector;
    detector.init(RED);
    cv::VideoCapture cap(media_path);
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

void YoloOption(const std::string &media_path) {
    ArmorDetector detector;
    detector.init(RED);

    cv::VideoCapture cap(media_path);
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


        //cv::waitKey(0);

        char c=cv::waitKey(30);
        if (c==27) break;


    }
    cap.release();
}

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        std::cout<<"----------\nUsage: [/main.cpp] [option] [media_path]\n----------"<<std::endl;
        std::cout<<"For More Options:\nNumber\t1 - Photo\nNumber\t2 - Video\nNumber\t3 - Yolo"<<std::endl;
        return -1;
    }

    //VideoOption();
    //PhotoOption();

    int optionX = std::stoi(argv[1]);
    std::string media_path = argv[2];

    switch (optionX) {
        case 1:
            PhotoOption(media_path);
            break;
        case 2:
            VideoOption(media_path);
            break;
        case 3:
            YoloOption(media_path);
            break;
        default:
            std::cout<<"Wrong Option: Please try again!"<<std::endl;
            return -2;
    }

    cv::waitKey(0);
}