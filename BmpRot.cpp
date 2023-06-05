#include <iostream>
#include <vector>
#include "Gradient.h"
#include "BMP.h"
#include "MeanShift.h"
#include "Opposition.h"
#include "Wave.h"
//#include "Interpolation.h"
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

//梯度先验
void GradientChecking(vector<unsigned char>& image, vector<unsigned char>& segmentation, BMPInfoHeader info_header) {
    float* gradient_map = new float[info_header.width * info_header.height];

    computeGradientMap(image, info_header, gradient_map);   //获取每个像素梯度

    //float* prior_map = new float[info_header.width * info_header.height];
    float a = 50.0f; // 先验权重
    //computePrior(gradient_map, info_header, a, prior_map);

    grabCut(image, gradient_map, info_header, a, segmentation);     //图像分割

    delete[]gradient_map;
}



int main() {
    string filename = "slg.bmp";
    BMPHeader header;
    BMPInfoHeader info_header;
    vector<unsigned char> image;    //输入像素数据
    //unsigned char* image = nullptr;
    vector<unsigned char> segmentation; //输出像素数据
    // 读取24bit BMP文件
    if (!readBMP(filename, image, header, info_header)) {
        return -1;
    }
    
    segmentation.resize(image.size());

    //GradientChecking(image, segmentation, info_header); //梯度先验
    //Opposition(image, segmentation);                        //反相
    //nearestInterpolation(image, segmentation, info_header.width, info_header.height);
    int width = info_header.width, height = info_header.height;
    
    //shock_filter(image, width, height, 10, 0.25f, 10.0f);
    //ShockFilter(std::vector<unsigned char> image, int width, int height, double standard_deviation, int threshold, int filter_size, int num_iterations);


    double sum = 0;
    for (unsigned char a : image) {
        sum += a;
    }
    /*
    标准差越小，激波波速越快，图像上的高频部分被更快地“震荡”掉，因此噪声被更好地去除，但也可能会导致一些细节被损坏；
      标准差越大，激波波速越慢，处理后的图像可能会更加平滑，但也许无法去除过多的噪声。建议初始值可以设置为0.1-0.3
    */
    double standard_deviation = 0.3f;

    /*
    阈值被用于限制震荡滤波器激波波速的最大值，大于该值的部分将被截断。在设置阈值时，应该考虑噪声的强度及其分布情况，
      通常建议将阈值设置为图像中像素强度的平均值加上标准差的两倍
    */
    int threshold = standard_deviation * 2 + sum / image.size();
    
    /*通常，较大的核能够去除较大的噪声，在保留较多图像细节的情况下，降低图像的噪声。建议使用3x3或5x5的核*/
    int filter_size = 3;

    //迭代次数越多，图像中的噪声将被更好地去除，但也会导致图像更加模糊。建议初次设置为2-3次
    int num_iterations = 2;
    ShockFilter sf(image, width, height, standard_deviation, threshold, filter_size, num_iterations);
    sf.filter();
    segmentation = sf.get_result();

    //segmentation = meanShift(image, width, height, 5, 5);
    

    // 写入BMP
    if (!writeBMP("test2.bmp", segmentation, header, info_header)) {
        return -1;
    }

    return 0;
}