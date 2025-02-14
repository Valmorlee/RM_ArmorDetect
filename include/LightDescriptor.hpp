//
// Created by valmorx on 25-2-14.
//

#ifndef LIGHTDESCRIPTOR_HPP
#define LIGHTDESCRIPTOR_HPP

#endif //LIGHTDESCRIPTOR_HPP

#include "tool.hpp"
#include "function.hpp"
#include "Center.hpp"
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

class LightDescriptor{

public:
    LightDescriptor();
    LightDescriptor(std::vector<cv::Point2f> points);
    LightDescriptor(cv::RotatedRect rect);
    ~LightDescriptor();


private:

    int color;
    int ll_x,ll_y,lr_x,lr_y;
    int rr_x,rr_y,rl_x,rl_y;

    Center center;

};