//
// Created by valmorx on 25-2-15.
//

#include<bits/stdc++.h>
#include "Parameter.hpp"
#include "tool.hpp"
#include "LightDescriptor.hpp"
#include "function.hpp"

Parameter::Parameter() {


    light_min_area = 200; //最小 [灯条矩形] 面积筛选参数
    light_max_ratio = 0.5; //最大 [灯条矩形] 长宽比
    light_contour_min_solidity = 0.1; //最小 [灯条轮廓椭圆] 拟合区域占比（凸度）
    light_area_extend_ratio = 1.2; //灯条面积扩展比
    light_max_angle_diff = 0.3; //最大灯条角度差
    light_max_angle_diff_ratio = 0.3; //最大灯条角度差比
    light_min_x_diff_ratio = 0.3; //最大灯条x轴差比
    light_max_y_diff_ratio = 0.3; //最大灯条y轴差比

    enemy_flag = 1; //发现敌方状态
    enemy_color = 0; //默认为蓝色

    armor_min_aspect_ratio = 0.5; //灯条相距距离与长度最大比值
    armor_max_aspect_ratio = 2.5; //灯条相距距离与长度最小比值
    armor_type_big_ratio = 1.5; //装甲板类型比值参数
}

void Parameter::set_enemy_color(int color) {
    this->enemy_color=color;
}

Parameter::~Parameter() = default;
