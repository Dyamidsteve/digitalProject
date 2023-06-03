
#include <fstream>
#include <iostream>

using namespace std;

// 该函数用于读取BMP格式的图像文件
// 参数：文件名
// 返回值：图像数据
char* readBMP(const char* filename) {
    // 打开文件
    ifstream file(filename, ios::binary);

    // 读取BMP文件头
    char header[54];
    file.read(header, 54);

    // 从文件头中提取图像宽度和高度
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // 分配内存存储图像数据
    int size = 3 * width * height;
    char* data = new char[size];

    // 读取图像数据
    file.read(data, size);

    // 关闭文件
    file.close();

    // 返回图像数据
    return data;
}