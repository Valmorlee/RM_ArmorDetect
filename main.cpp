//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include "function.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>
#include "ArmorDetector.hpp"

int main() {
    cv::Mat img = cv::imread("../PhotoLib/Armor03.jpg"); // 读取图像
    cv::imshow("img", img);

    if (img.empty()) {
        std::cout << "Empty" << std::endl;
        return -1;
    }

    ArmorDetector detector;
    func_armorDetect(img,detector);

    cv::waitKey(0);
}