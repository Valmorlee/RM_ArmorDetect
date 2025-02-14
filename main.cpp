//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include "function.hpp"
#include "Center.hpp"
#include "LightDescriptor.hpp"
#include<bits/stdc++.h>
#include<opencv2/opencv.hpp>

int main() {
    cv::Mat img = cv::imread("../PhotoLib/Armor01.jpg"); // 读取图像

    if (img.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    cv::imshow("img",img);
    func_armorDetect(img);
}