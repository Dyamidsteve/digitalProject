#include <iostream>
#include <vector>
#include "BMP.h"
using namespace std;

// ͼ���С
const int WIDTH = 640;
const int HEIGHT = 480;

// �ݶ������㷨
void gradientPrior(vector<unsigned char>& image) {
    vector<double> gradient(image.size()); // �洢�ݶ���Ϣ
    // ���������ݶ�
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            int index = (y * WIDTH + x) * 3; // û������ռ��3���ֽڣ������ǿ���һ������
            double dx = image[index + 3] - image[index - 3]; // ����x�����ϵ��ݶ�
            double dy = image[index + WIDTH * 3] - image[index - WIDTH * 3]; // ����y�����ϵ��ݶ�
            gradient[y * WIDTH + x] = sqrt(dx * dx + dy * dy);
        }
    }

    // ������ֵ�����޸�
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int index = (y * WIDTH + x) * 3;
            double alpha = gradient[y * WIDTH + x];
            if (alpha <= 1.0) {
                continue; // �ݶȺܵͣ������޸�����ֵ
            }
            int sum = image[index] + image[index + 1] + image[index + 2];
            int newValue = (sum / 3) * ((int)(alpha * alpha) / 10000); // �޸�����ֵ
            image[index] = newValue;
            image[index + 1] = newValue;
            image[index + 2] = newValue;
        }
    }
}

int main() {
    string filename = "ddd.bmp";
    BMPHeader header;
    BMPInfoHeader info_header;
    vector<unsigned char> image;
    // �˴�ʡ�Զ�image�Ķ�ȡ����
    if (!readBMP(filename, image, header, info_header)) {
        return -1;
    }
    gradientPrior(image);
    // �˴�ʡ�Զ�image�ı������
    if (!writeBMP("test.bmp", image, header, info_header)) {
        return -1;
    }
    return 0;
}