#pragma once
#include <vector>
#include <iostream>

// �������ص�ṹ��
struct Pixel
{
    int r, g, b;
};

// �������ؾ���ĺ���
float calculate_distance(const Pixel& p1, const Pixel& p2);
// ��ֵƯ�ƺ���
Pixel mean_shift(const std::vector<Pixel>& pixels, float bandwidth);
// �ָ�ͼ����
std::vector<std::vector<Pixel>> image_segmentation(const std::vector<unsigned char>& image, int width, int height, float bandwidth);
