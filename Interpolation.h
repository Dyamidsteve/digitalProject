#pragma once
#include<vector>
#include<iostream>

using namespace std;

// 最近邻插值法重建图像
void nearestInterpolation(vector<unsigned char>& image, vector<unsigned char>& segmentation, int width, int height);