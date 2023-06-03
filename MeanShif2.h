#pragma once
#include<vector>
#include<iostream>

typedef unsigned char byte;

// 计算均值漂移向量函数
void mean_shift_vector(const std::vector<byte>& image, int rows, int cols, double x, double y, double& shift_x, double& shift_y, double spatial_radius, double color_radius);

// 计算均值漂移向量(迭代函数)
void mean_shift_iteration(const std::vector<byte>& image, int rows, int cols, double x, double y, double& new_x, double& new_y, double spatial_radius, double color_radius);

// 图像分割函数
void mean_shift_segmentation(std::vector<unsigned char>& image, int height, int width, int channel, int spatial_radius, int color_radius, std::vector<unsigned char>& result);