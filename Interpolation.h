#pragma once
#include<vector>
#include<iostream>
#include<algorithm>
#include<cmath>

const int WAVELET_SIZE = 2; // 小波窗口大小

using namespace std;

// 最近邻插值法重建图像
void nearestInterpolation(vector<unsigned char>& image, vector<unsigned char>& segmentation, int width, int height);

// 计算 Haar 小波变换的基础函数
void haarWavelet(vector<double>& data, int size);

// 对一维数据进行 Haar 小波变换
void haarWavelet1D(std::vector<double>& data);

// 对二维数据进行 Haar 小波变换
void haarWavelet2D(std::vector<std::vector<double>>& data);

// 对二维数据进行 Haar 小波逆变换
void haarInverseWavelet2D(std::vector<std::vector<double>>& data);

// 对24位BMP图像进行模糊
void waveletBlur(std::vector<unsigned char>& image, int width, int height);