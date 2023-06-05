#include "BMP.h"

// ���ļ��ж�ȡ BMP ͼ�񲢷���ͼ�����ݣ�header �� info_header �洢�ļ�ͷ����Ϣͷ
bool readBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }
    fin.read((char*)&header, sizeof(BMPHeader));
    fin.read((char*)&info_header, sizeof(BMPInfoHeader));
    // ��֧����ѹ����24 bpp �� BMP ͼ��
    if (info_header.compress_type != 0 || info_header.bpp != 24) {
        cout << "Unsupported BMP format." << endl;
        fin.close();
        return false;
    }
    // ����ÿһ�����ص��ֽ�������Ҫ���뵽 4 �ı�����
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // ��ȡͼ������
    image.resize(info_header.height * row_size);
    fin.seekg(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fin.read((char*)&image[row * row_size], row_size);
    }
    fin.close();
    return true;
}

bool readBMP2(string filename, unsigned char* image, BMPHeader& header, BMPInfoHeader& info_header) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }
    fin.read((char*)&header, sizeof(BMPHeader));
    fin.read((char*)&info_header, sizeof(BMPInfoHeader));
    // ��֧����ѹ����24 bpp �� BMP ͼ��
    if (info_header.compress_type != 0 || info_header.bpp != 24) {
        cout << "Unsupported BMP format." << endl;
        fin.close();
        return false;
    }
    // ����ÿһ�����ص��ֽ�������Ҫ���뵽 4 �ı�����
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // ��ȡͼ������
    image = new unsigned char[info_header.height * row_size];
    fin.seekg(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fin.read((char*)&image[row * row_size], row_size);
    }
    fin.close();
    return true;
}

// ��ͼ�����ݱ���Ϊ BMP �ļ���header �� info_header �洢�ļ�ͷ����Ϣͷ
bool writeBMP(string filename, vector<unsigned char>& image, BMPHeader& header, BMPInfoHeader& info_header) {
    ofstream fout(filename, ios::binary);
    if (!fout) {
        cout << "Failed to create file: " << filename << endl;
        return false;
    }
    // ����ÿһ�����ص��ֽ�������Ҫ���뵽 4 �ı�����
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // �޸� BMP Header �� BMP Info Header
    header.size = header.offset + info_header.height * row_size;
    info_header.bmp_size = header.size - header.offset;
    // д�� BMP Header �� BMP Info Header
    fout.write((char*)&header, sizeof(BMPHeader));
    fout.write((char*)&info_header, sizeof(BMPInfoHeader));
    // д��ͼ������
    fout.seekp(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fout.write((char*)&image[row * row_size], row_size);
    }
    fout.close();
    return true;
}

bool writeBMP2(string filename, unsigned char* image, BMPHeader& header, BMPInfoHeader& info_header) {
    ofstream fout(filename, ios::binary);
    if (!fout) {
        cout << "Failed to create file: " << filename << endl;
        return false;
    }
    // ����ÿһ�����ص��ֽ�������Ҫ���뵽 4 �ı�����
    int row_size = (info_header.width * info_header.bpp / 8 + 3) / 4 * 4;
    // �޸� BMP Header �� BMP Info Header
    header.size = header.offset + info_header.height * row_size;
    info_header.bmp_size = header.size - header.offset;
    // д�� BMP Header �� BMP Info Header
    fout.write((char*)&header, sizeof(BMPHeader));
    fout.write((char*)&info_header, sizeof(BMPInfoHeader));
    // д��ͼ������
    fout.seekp(header.offset, ios::beg);
    for (int row = info_header.height - 1; row >= 0; --row) {
        fout.write((char*)&image[row * row_size], row_size);
    }
    fout.close();
    return true;
}