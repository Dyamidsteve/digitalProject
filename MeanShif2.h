#pragma once
#include<vector>
#include<iostream>

typedef unsigned char byte;

// �����ֵƯ����������
void mean_shift_vector(const std::vector<byte>& image, int rows, int cols, double x, double y, double& shift_x, double& shift_y, double spatial_radius, double color_radius);

// �����ֵƯ������(��������)
void mean_shift_iteration(const std::vector<byte>& image, int rows, int cols, double x, double y, double& new_x, double& new_y, double spatial_radius, double color_radius);

// ͼ��ָ��
void mean_shift_segmentation(std::vector<unsigned char>& image, int height, int width, int channel, int spatial_radius, int color_radius, std::vector<unsigned char>& result);