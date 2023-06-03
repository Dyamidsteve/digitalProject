#pragma once
#include<vector>
#include<iostream>
#include<cmath>

using namespace std;

// ����һ���˲���
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

    // �����ݶȷ���
    void calculate_gradient_magnitude();

    // �����ݶȷ��ȵı�׼��
    double calculate_standard_deviation();

    // �ж������Ƿ����ڱ�Ե
    bool is_edge_pixel(int i, int j);

    // ����Shock Value
    void calculate_shock_value();

    // ��ԭͼ������˲�����
    void filter_image();
};

