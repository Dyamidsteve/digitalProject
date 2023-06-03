#pragma once
#include<vector>
#include<iostream>
#include<cmath>

using namespace std;

// 定义一个滤波器
class ShockFilter {
public:
    ShockFilter(std::vector<unsigned char> image, int width, int height, double standard_deviation, int threshold, int filter_size, int num_iterations);
    void filter();
    std::vector<unsigned char> get_result();

private:
    std::vector<unsigned char> image_;
    int width_;
    int height_;
    double standard_deviation_;
    int threshold_;
    int filter_size_;
    int num_iterations_;
    std::vector<double> gradient_magnitude_;
    std::vector<double> shock_value_;

    // 计算梯度幅度
    void calculate_gradient_magnitude();

    // 计算梯度幅度的标准差
    double calculate_standard_deviation();

    // 判断像素是否属于边缘
    bool is_edge_pixel(int i, int j);

    // 计算Shock Value
    void calculate_shock_value();

    // 对原图像进行滤波处理
    void filter_image();
};

