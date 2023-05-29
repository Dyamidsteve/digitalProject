#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

#pragma pack(push,1) //����1�ֽڶ���
struct BMPHeader {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
};

struct BMPInfoHeader {
    unsigned int size;               //DIB header��С
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bpp;              //ÿ�����ص��λ��
    unsigned int compress_type;
    unsigned int bmp_size;
    unsigned int h_res;
    unsigned int v_res;
    unsigned int color_num;
    unsigned int important_color_num;
};

typedef struct tagRGBQUAD {
    unsigned char    rgbBlue;  // ��ɫ����
    unsigned char    rgbGreen; // ��ɫ����
    unsigned char    rgbRed;   // ��ɫ����
    unsigned char   rgbReserved; // �����ֽڣ�δʹ�ã�
} RGBQUAD;
#pragma pack(pop) //�ָ����뷽ʽ

// ���ļ��ж�ȡBMPͼ�񲢷���ͼ������
bool readBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header);

// ��ͼ�����ݱ���Ϊ BMP �ļ�
bool writeBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header);
