#include <iostream>
#include <vector>
#include "Gradient.h"
#include "BMP.h"
#include "MeanShift.h"
#include "Opposition.h"
#include "Wave.h"
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



int main() {
    string filename = "bb.bmp";
    BMPHeader header;
    BMPInfoHeader info_header;
    vector<unsigned char> image;    //������������
    //unsigned char* image = nullptr;
    vector<unsigned char> segmentation; //�����������
    // ��ȡ24bit BMP�ļ�
    if (!readBMP(filename, image, header, info_header)) {
        return -1;
    }
    
    segmentation.resize(image.size());

    //GradientChecking(image, segmentation, info_header); //�ݶ�����
    //Opposition(image, segmentation);                        //����
    //nearestInterpolation(image, segmentation, info_header.width, info_header.height);
    int width = info_header.width, height = info_header.height;
    //shock_filter(image, width, height, 10, 0.25f, 10.0f);
    //ShockFilter(std::vector<unsigned char> image, int width, int height, double standard_deviation, int threshold, int filter_size, int num_iterations);


    /*
    double standard_deviation = 0.01f;
    int threshold = 10;
    int filter_size = 3;
    int num_iterations = 2;
    ShockFilter sf(image,width,height,standard_deviation,threshold,filter_size,num_iterations);
    sf.filter();
    segmentation = sf.get_result();
    */

    segmentation = meanShift(image, width, height, 310.20f, 310.20f);
    

    // д��BMP
    if (!writeBMP("test2.bmp", segmentation, header, info_header)) {
        return -1;
    }

    return 0;
}