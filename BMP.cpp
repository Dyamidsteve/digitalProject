#include "BMP.h"

// 从文件中读取 BMP 图像并返回图像数据，header 和 info_header 存储文件头和信息头
bool readBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }
    fin.read((char*)&header, sizeof(BMPHeader));
    fin.read((char*)&info_header, sizeof(BMPInfoHeader));
    // 仅支持无压缩、24 bpp 的 BMP 图像
    if (info_header.compress_type != 0 || info_header.bpp != 24) {
        cout << "Unsupported BMP format." << endl;
        fin.close();
        return false;
    }
    // 计算每一行像素的字节数（需要补齐到 4 的倍数）
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // 读取图像数据
    image.resize(info_header.height * row_size);
    fin.seekg(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fin.read((char*)&image[row * row_size], row_size);
    }
    fin.close();
    return true;
}

// 将图像数据保存为 BMP 文件，header 和 info_header 存储文件头和信息头
bool writeBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header) {
    ofstream fout(filename, ios::binary);
    if (!fout) {
        cout << "Failed to create file: " << filename << endl;
        return false;
    }
    // 计算每一行像素的字节数（需要补齐到 4 的倍数）
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // 修改 BMP Header 和 BMP Info Header
    header.size = header.offset + info_header.height * row_size;
    info_header.bmp_size = header.size - header.offset;
    // 写入 BMP Header 和 BMP Info Header
    fout.write((char*)&header, sizeof(BMPHeader));
    fout.write((char*)&info_header, sizeof(BMPInfoHeader));
    // 写入图像数据
    fout.seekp(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fout.write((char*)&image[row * row_size], row_size);
    }
    fout.close();
    return true;
}