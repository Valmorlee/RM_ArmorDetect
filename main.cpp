//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include "function.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "ArmorDetector.hpp"
#include <time.h>
#include <sys/time.h>

int main() {
    cv::Mat img = cv::imread("../PhotoLib/Armor03.jpg"); // 读取图像
    cv::imshow("img", img);

    if (img.empty()) {
        std::cout << "Empty" << std::endl;
        return -1;
    }

    ArmorDetector detector;
    detector.init(RED);
    detector.loadImg(img);

    func_armorDetect(img,detector);
    // cv::Mat imgx = detector._armors[0].frontImg;
    // cv::resize(imgx,imgx,cv::Size(400,400));
    //
    // cv::imshow("imgx", imgx);


    cv::waitKey(0);
}