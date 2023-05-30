#pragma once
#include <vector>
#include <iostream>

// 定义像素点结构体
struct Pixel
{
    int r, g, b;
};

// 计算像素距离的函数
float calculate_distance(const Pixel& p1, const Pixel& p2);
// 均值漂移函数
Pixel mean_shift(const std::vector<Pixel>& pixels, float bandwidth);
// 分割图像函数
std::vector<std::vector<Pixel>> image_segmentation(const std::vector<unsigned char>& image, int width, int height, float bandwidth);
