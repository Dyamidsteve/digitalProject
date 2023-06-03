#pragma once
#include<vector>
#include<iostream>
#include<algorithm>
#include<cmath>

const int WAVELET_SIZE = 2; // С�����ڴ�С

using namespace std;

// ����ڲ�ֵ���ؽ�ͼ��
void nearestInterpolation(vector<unsigned char>& image, vector<unsigned char>& segmentation, int width, int height);

// ���� Haar С���任�Ļ�������
void haarWavelet(vector<double>& data, int size);

// ��һά���ݽ��� Haar С���任
void haarWavelet1D(std::vector<double>& data);

// �Զ�ά���ݽ��� Haar С���任
void haarWavelet2D(std::vector<std::vector<double>>& data);

// �Զ�ά���ݽ��� Haar С����任
void haarInverseWavelet2D(std::vector<std::vector<double>>& data);

// ��24λBMPͼ�����ģ��
void waveletBlur(std::vector<unsigned char>& image, int width, int height);