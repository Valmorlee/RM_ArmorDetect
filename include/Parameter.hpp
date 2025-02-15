//
// Created by valmorx on 25-2-15.
//
#pragma once
#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#endif //PARAMETER_HPP

class Parameter {
public:
    Parameter();
    ~Parameter();

    // 颜色相关参数
    int enemy_color; // 0: blue, 1: red
    void set_enemy_color(int enemy_color);


    // 灯条相关参数
    double light_min_area; //最小 [灯条矩形] 面积筛选参数
    double light_max_ratio; //最大 [灯条矩形] 长宽比
    double light_contour_min_solidity; //最小 [灯条轮廓椭圆] 拟合区域占比
    double light_area_extend_ratio; //灯条面积扩展比
    double light_max_angle_diff; //灯条最大角度差
    double light_max_angle_diff_ratio; //灯条最大角度差比
    double light_max_y_diff_ratio; //灯条最大y轴差比
    double light_min_x_diff_ratio; //灯条最大x轴差比


    // 状态指标相关参数
    int enemy_flag;

    // 装甲板相关参数
    double armor_max_aspect_ratio; //灯条相距距离与长度最大比值
    double armor_min_aspect_ratio; //灯条相距距离与长度最小比值
    double armor_type_big_ratio; //大装甲板类型比值参数
    double armor_type_small_ratio; //小装甲板类型比值参数
};