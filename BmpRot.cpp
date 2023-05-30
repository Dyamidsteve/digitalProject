#include <iostream>
#include <vector>
#include "Gradient.h"
#include "BMP.h"
#include "MeanShift.h"
using namespace std;

// ͼ���С
const int WIDTH = 640;
const int HEIGHT = 480;

 //�ݶ������㷨
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

//�ݶ�����
void GradientChecking(vector<unsigned char>& image, vector<unsigned char>& segmentation, BMPInfoHeader info_header) {
    float* gradient_map = new float[info_header.width * info_header.height];

    computeGradientMap(image, info_header, gradient_map);   //��ȡÿ�������ݶ�

    //float* prior_map = new float[info_header.width * info_header.height];
    float a = 50.0f; // ����Ȩ��
    //computePrior(gradient_map, info_header, a, prior_map);

    grabCut(image, gradient_map, info_header, a, segmentation);     //ͼ��ָ�

    delete[]gradient_map;
}

//��ֵƯ��
void MeanShift(vector<unsigned char>& image, vector<unsigned char>& segmented_image, BMPInfoHeader info_header) {
    int height = info_header.height, width = info_header.width;

    // �����������ͷָ���ֵ
    float bandwidth = 30.0f;
    int threshold = 1;

    // ����ͼ��ָ�
    vector<vector<Pixel>> segments = image_segmentation(image, width, height, bandwidth);

    // ���ָ�������Ϊͼ��
    segmented_image = vector<unsigned char>(image.size(), 255);
    for (const auto& segment : segments)
    {
        //continue;
        if (segment.size() < threshold)
        {
            continue;
        }
        Pixel color{ rand() % 256, rand() % 256, rand() % 256 };
        for (const auto& pixel : segment)
        {
            segmented_image[pixel.r + pixel.g * width + pixel.b * width * height] = color.r;
            segmented_image[pixel.r + pixel.g * width + pixel.b * width * height + 1] = color.g;
            segmented_image[pixel.r + pixel.g * width + pixel.b * width * height + 2] = color.b;
        }
    }

}

int main() {
    string filename = "bb.bmp";
    BMPHeader header;
    BMPInfoHeader info_header;
    vector<unsigned char> image;    //������������
    vector<unsigned char> segmentation; //�����������
    // ��ȡ24bit BMP�ļ�
    if (!readBMP(filename, image, header, info_header)) {
        return -1;
    }
    
    segmentation.resize(image.size());

    //GradientChecking(image, segmentation, info_header); //�ݶ�����
    MeanShift(image, segmentation, info_header);          //��ֵƯ��

    // д��BMP
    if (!writeBMP("test2.bmp", segmentation, header, info_header)) {
        return -1;
    }

    return 0;
}