#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

#pragma pack(push,1) //设置1字节对齐
struct BMPHeader {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
};

struct BMPInfoHeader {
    unsigned int size;               //DIB header大小
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bpp;              //每个像素点的位数
    unsigned int compress_type;
    unsigned int bmp_size;
    unsigned int h_res;
    unsigned int v_res;
    unsigned int color_num;
    unsigned int important_color_num;
};

typedef struct tagRGBQUAD {
    unsigned char    rgbBlue;  // 蓝色分量
    unsigned char    rgbGreen; // 绿色分量
    unsigned char    rgbRed;   // 红色分量
    unsigned char   rgbReserved; // 保留字节（未使用）
} RGBQUAD;
#pragma pack(pop) //恢复对齐方式

// 从文件中读取BMP图像并返回图像数据
bool readBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header);

bool readBMP2(string filename, unsigned char* image, BMPHeader& header, BMPInfoHeader& info_header);

// 将图像数据保存为 BMP 文件
bool writeBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header);

bool writeBMP2(string filename, unsigned char* image, BMPHeader& header, BMPInfoHeader& info_header);
