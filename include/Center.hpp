//
// Created by valmorx on 25-2-14.
//
#pragma once
#ifndef CENTER_HPP
#define CENTER_HPP

#endif //CENTER_HPP

class Center {
public:
    Center();
    Center(double x, double y);

    double get_x() const {
        return x;
    }

    void set_x(int x) {
        this->x = x;
    }

    double get_y() const {
        return y;
    }

    void set_y(int y) {
        this->y = y;
    }

    ~Center();

private:
    double x;
    double y;
};