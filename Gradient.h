#pragma once
#include "BMP.h"
#include <vector>
using namespace std;

// �������ص���ݶ�
float gradient(vector<unsigned char>& image, int x, int y, int width);

// ����ÿ�����ص���ݶ�ֵ
void computeGradientMap(vector<unsigned char>& image, BMPInfoHeader& info_header, float* gradient_map);

// �����ݶ�����ģ��
void computePrior(float* gradient_map, BMPInfoHeader& info_header, float a, float* prior_map);

// ִ���ݶ������㷨������ͼ��ָ���
void grabCut(vector<unsigned char>& image, float* gradient_map, BMPInfoHeader& info_header, float a, vector<unsigned char>& segmentation);