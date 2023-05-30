#pragma once
#include "BMP.h"
#include <vector>
using namespace std;

// 计算像素点的梯度
float gradient(vector<unsigned char>& image, int x, int y, int width);

// 计算每个像素点的梯度值
void computeGradientMap(vector<unsigned char>& image, BMPInfoHeader& info_header, float* gradient_map);

// 计算梯度先验模型
void computePrior(float* gradient_map, BMPInfoHeader& info_header, float a, float* prior_map);

// 执行梯度先验算法，生成图像分割结果
void grabCut(vector<unsigned char>& image, float* gradient_map, BMPInfoHeader& info_header, float a, vector<unsigned char>& segmentation);