#include "Gradient.h"
#include "BMP.h"


// �������ص���ݶ�
float gradient(vector<unsigned char>& image, int x, int y, int width) {
    float dx = image[x * width + y + 1] - image[x * width + y];
    float dy = image[(x + 1) * width + y] - image[x * width + y];
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

// ����ÿ�����ص���ݶ�ֵ
void computeGradientMap(vector<unsigned char>& image, BMPInfoHeader& info_header, float* gradient_map) {
    int width = info_header.width;
    int height = info_header.height;

    //��ʼ����Ե�ݶ�Ϊ0
    //��ֱ��Ե
    for (int i = 0; i < height; i++) {
        gradient_map[i * width] = 0;
        gradient_map[(i + 1) * width - 1] = 0;
    }

    //ˮƽ��Ե
    for (int i = 0; i < width; i++) {
        gradient_map[i] = 0;
        gradient_map[(height - 1) * width + i] = 0;
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            float g = gradient(image, i, j, width);
            gradient_map[i * width + j] = g;
            //if (g != 0) {
            //    cout << "gradient:" << gradient_map[i * width + j] << endl;
            //}
        }
    }
}

// �����ݶ�����ģ��
void computePrior(float* gradient_map, BMPInfoHeader& info_header, float a, float* prior_map) {
    int width = info_header.width;
    int height = info_header.height;


    //
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            float gp = gradient_map[i * width + j];
            float up = gradient_map[(i - 1) * width + j];
            float down = gradient_map[(i + 1) * width + j];
            float left = gradient_map[i * width + j - 1];
            float right = gradient_map[i * width + j + 1];

            float u = a * max(gp - up, 0.0f);
            float d = a * max(gp - down, 0.0f);
            float l = a * max(gp - left, 0.0f);
            float r = a * max(gp - right, 0.0f);

            prior_map[i * width + j] = u + d + l + r;
        }
    }
}

// ִ���ݶ������㷨������ͼ��ָ���
void grabCut(vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header, float* prior_map, vector<unsigned char>& segmentation) {
    int width = info_header.width;
    int height = info_header.height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (prior_map[i * width + j] >= 10) {
                segmentation[i * width + j] = 255;
            }
            else {
                segmentation[i * width + j] = 0;
            }
        }
    }
}

/*
// ����ͼ���ݶ�
void computeGradient(const vector<unsigned char>& image,vector<int>& gradient){
    gradient.resize(WIDTH * HEIGHT);
    for(int y = 1;y < HEIGHT - 1;y++){
        for(int x = 1;x < WIDTH - 1;x++){
            int dx = image[(y * WIDTH + x + 1) * 3] - image[(y * WIDTH + x - 1) * 3];
            int dy = image[((y + 1) * WIDTH + x) * 3] - image[((y - 1) * WIDTH + x) * 3];
            gradient[y * WIDTH + x] = sqrt(dx * dx + dy * dy);
        }
    }
}

// ����ǰ��������ֵ
int binarize(const vector<int>& gradient,double beta){
    int hist[256],total = 0;
    memset(hist,0,sizeof(hist));
    for(int i = 1;i < HEIGHT - 1;i++){
        for(int j = 1;j < WIDTH - 1;j++){
            if(gradient[i * WIDTH + j] > 0){
                hist[gradient[i * WIDTH + j]]++;
                total++;
            }
        }
    }
    double sum = 0;
    for(int t = 1;t <= 255;t++){
        sum += t * (double)hist[t] / total;
    }
    int threshold = (int)(sum / beta);
    return threshold;
}

// ��ǰ�����Ϊ��ɫ���������Ϊ��ɫ
void segmentImage(const vector<int>& gradient,vector<unsigned char>& result,int threshold){
    result.resize(WIDTH * HEIGHT * 3);
    for(int i = 0;i < HEIGHT;i++){
        for(int j = 0;j < WIDTH;j++){
            if(gradient[i * WIDTH + j] >= threshold){
                result[(i * WIDTH + j) * 3] = 255;
                result[(i * WIDTH + j) * 3 + 1] = 255;
                result[(i * WIDTH + j) * 3 + 2] = 255;
            }else{
                result[(i * WIDTH + j) * 3] = 0;
                result[(i * WIDTH + j) * 3 + 1] = 0;
                result[(i * WIDTH + j) * 3 + 2] = 0;
            }
        }
    }
}
*/

//int main() {
//
//    string filename = "ddd.bmp";
//    BMPHeader header;
//    BMPInfoHeader info_header;
//    vector<unsigned char> image;
//    vector<unsigned char> segmentation;
//    //cout << "reading***" << endl;
//    if (!readBMP(filename, image, header, info_header)) {
//        return -1;
//    }
//    //writeBMP("cc.bmp", image, header, info_header);
//    //return -1;
//    segmentation.resize(image.size());
//
//    float* gradient_map = new float[info_header.width * info_header.height];
//    computeGradientMap(image, info_header, gradient_map);
//
//    float* prior_map = new float[info_header.width * info_header.height];
//    float a = 0.02; // ����Ȩ��
//    computePrior(gradient_map, info_header, a, prior_map);
//
//    grabCut(image, header, info_header, prior_map, segmentation);
//
//    //for (int i = 0;i<segmentation.size();i++) {
//    //    segmentation[i] = 125;
//    //}
//
//    delete[] gradient_map;
//    delete[] prior_map;
//
//    if (!writeBMP("segmentation2.bmp", segmentation, header, info_header)) {
//        cout << "error" << endl;
//        return -1;
//    }
//
//    return 0;
//}