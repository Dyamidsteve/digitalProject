#include <iostream>
#include <vector>
#include "Gradient.h"
#include "BMP.h"
#include "MeanShift.h"
#include "Opposition.h"
//#include "Interpolation.h"
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

//��ֵƯ��1
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
            int index = 3 * (pixel.r + pixel.g * width + pixel.b * width * height);//pixel.r + pixel.g * width + pixel.b * width * height;
            segmented_image[index] = color.r;
            segmented_image[index+1] = color.g;
            segmented_image[index+2]= color.b;
        }
    }

}

////��ֵƯ��1
//void MeanShift2(vector<unsigned char>& image, vector<unsigned char>& segmented_image, BMPInfoHeader info_header) {
//    int height = info_header.height, width = info_header.width;
//
//    // �����������ͷָ���ֵ
//    int channel = 3;
//    int spatial_radius = 8;
//    int color_radius = 16;
//
//    mean_shift_segmentation(image, height, width, channel, spatial_radius, color_radius, segmented_image);
//
//    //// ͼ��ָ��
//    //void mean_shift_segmentation(std::vector<unsigned char>&image, int height, int width, int channel, int spatial_radius, int color_radius, std::vector<unsigned char>&result)
//
//}

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

    GradientChecking(image, segmentation, info_header); //�ݶ�����
    //MeanShift(image, segmentation, info_header);          //��ֵƯ��
    //MeanShift2(image, segmentation, info_header);          //��ֵƯ��
    //Opposition(image, segmentation);                        //����
    //nearestInterpolation(image, segmentation, info_header.width, info_header.height);
    //waveletBlur(image, info_header.width, info_header.height);

    // д��BMP
    if (!writeBMP("test2.bmp", segmentation, header, info_header)) {
        return -1;
    }

    return 0;
}