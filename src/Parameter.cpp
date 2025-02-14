//
// Created by valmorx on 25-2-15.
//

#include<bits/stdc++.h>
#include "Parameter.hpp"
#include "tool.hpp"
#include "LightDescriptor.hpp"
#include "Center.hpp"
#include "function.hpp"

Parameter::Parameter() {
    light_min_area = 1000; //最小 [灯条矩形] 面积筛选参数
    light_max_ratio = 0.2; //最大 [灯条矩形] 长宽比
    light_contour_min_solidity = 1.1; //最小 [灯条轮廓椭圆] 拟合区域占比
    light_area_extend_ratio = 1.2; //灯条面积扩展比
}

Parameter::~Parameter() = default;
