//
// Created by valmorx on 25-2-15.
//

#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#endif //PARAMETER_HPP

#pragma once
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

    // 状态指标相关参数
    int enemy_flag;

};