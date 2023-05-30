#include <iostream>
#include <vector>
#include "Gradient.h"
#include "BMP.h"
using namespace std;

// 图像大小
const int WIDTH = 640;
const int HEIGHT = 480;

 //梯度先验算法
void gradientPrior(vector<unsigned char>& image) {
    vector<double> gradient(image.size()); // 存储梯度信息
    // 计算像素梯度
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            int index = (y * WIDTH + x) * 3; // 没个像素占用3个字节，把它们看作一个数列
            double dx = image[index + 3] - image[index - 3]; // 计算x方向上的梯度
            double dy = image[index + WIDTH * 3] - image[index - WIDTH * 3]; // 计算y方向上的梯度
            gradient[y * WIDTH + x] = sqrt(dx * dx + dy * dy);
        }
    }

    // 对像素值进行修改
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int index = (y * WIDTH + x) * 3;
            double alpha = gradient[y * WIDTH + x];
            if (alpha <= 1.0) {
                continue; // 梯度很低，无需修改像素值
            }
            int sum = image[index] + image[index + 1] + image[index + 2];
            int newValue = (sum / 3) * ((int)(alpha * alpha) / 10000); // 修改像素值
            image[index] = newValue;
            image[index + 1] = newValue;
            image[index + 2] = newValue;
        }
    }
}

int main() {
    string filename = "bb.bmp";
    BMPHeader header;
    BMPInfoHeader info_header;
    vector<unsigned char> image;
    vector<unsigned char> segmentation;
    // 此处省略对image的读取过程
    if (!readBMP(filename, image, header, info_header)) {
        return -1;
    }
    
    segmentation.resize(image.size());

    float* gradient_map = new float[info_header.width * info_header.height];
    
    computeGradientMap(image, info_header, gradient_map);

    //float* prior_map = new float[info_header.width * info_header.height];
    float a = 50.0f; // 先验权重
    //computePrior(gradient_map, info_header, a, prior_map);

    grabCut(image, gradient_map, info_header, a, segmentation);     //图像分割

    // 此处省略对image的保存过程
    if (!writeBMP("test.bmp", segmentation, header, info_header)) {
        return -1;
    }

    delete[] gradient_map;
    return 0;
}