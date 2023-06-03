#include "Interpolation.h"

void nearestInterpolation(vector<unsigned char>& image, vector<unsigned char>& segmentation, int width, int height) {


    float x_ratio = float(width) / width;
    float y_ratio = float(height) / height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x = int(j * x_ratio);
            int y = int(i * y_ratio);
            int index = (y * width + x) * 3;

            int newIndex = (i * width + j) * 3;

            segmentation[newIndex] = image[index];
            segmentation[newIndex + 1] = image[index + 1];
            segmentation[newIndex + 2] = image[index + 2];
        }
    }

}



// 计算 Haar 小波变换的基础函数
void haarWavelet(vector<double>& data, int size) {
    int halfSize = size / 2;
    double *temp = new double[size];

    for (int i = 0; i < halfSize; i++) {
        temp[i] = (data[2 * i] + data[2 * i + 1]) / sqrt(2.0);
        temp[i + halfSize] = (data[2 * i] - data[2 * i + 1]) / sqrt(2.0);
    }
    std::copy(temp, temp + size, data);
    delete[]temp;
}

// 对一维数据进行 Haar 小波变换
void haarWavelet1D(std::vector<double>& data) {
    int size = data.size();

    // 对每个分辨率下的数据进行小波变换
    for (int i = size; i >= 2; i /= 2) {
        vector<double>temp;
        temp.resize(size);
        std::copy(data.begin(), data.begin() + i, temp);

        haarWavelet(temp, i);

        std::copy(temp.begin(), temp.begin() + i, data.begin());
    }
}

// 对二维数据进行 Haar 小波变换
void haarWavelet2D(std::vector<std::vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();
    std::vector<double> row(cols);

    // 对每行数据进行小波变换
    for (int i = 0; i < rows; i++) {
        std::copy(data[i].begin(), data[i].end(), row.begin());
        haarWavelet1D(row);
        std::copy(row.begin(), row.end(), data[i].begin());
    }

    // 对每列数据进行小波变换
    std::vector<double> col(rows);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            col[i] = data[i][j];
        }

        haarWavelet1D(col);

        for (int i = 0; i < rows; i++) {
            data[i][j] = col[i];
        }
    }
}

// 对二维数据进行 Haar 小波逆变换
void haarInverseWavelet2D(std::vector<std::vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();
    std::vector<double> col(rows);

    // 对每列数据进行小波逆变换
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            col[i] = data[i][j];
        }

        haarWavelet(col, rows);

        for (int i = 0; i < rows; i++) {
            data[i][j] = col[i];
        }
    }

    // 对每行数据进行小波逆变换
    std::vector<double> row(cols);
    for (int i = 0; i < rows; i++) {
        std::copy(data[i].begin(), data[i].end(), row.begin());
        haarWavelet(row, cols);
        std::copy(row.begin(), row.end(), data[i].begin());
    }
}

// 对24位BMP图像进行模糊
void waveletBlur(std::vector<unsigned char>& image, int width, int height) {
    int pixelByteCount = 3;
    int imageWidthBytes = width * pixelByteCount;
    int imageHeightBytes = height * imageWidthBytes;
    int rows = height;
    int cols = imageWidthBytes;
    std::vector<std::vector<double>> imageData(rows, std::vector<double>(cols));

    // 将图像数据转换为数据矩阵
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < imageWidthBytes; j++) {
            imageData[i][j] = image[i * imageWidthBytes + j];
        }
    }

    // 对数据矩阵进行小波变换
    haarWavelet2D(imageData);

    // 用低频分量
    int blurRadius = 3;

    // 对低频分量进行模糊处理
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i < blurRadius || i >= rows - blurRadius || j < blurRadius * pixelByteCount || j >= cols - blurRadius * pixelByteCount) {
                // 边缘像素不做处理
                continue;
            }

            double sum = 0.0;
            for (int k = -blurRadius; k <= blurRadius; k++) {
                for (int l = -blurRadius * pixelByteCount; l <= blurRadius * pixelByteCount; l++) {
                    sum += imageData[i + k][j + l];
                }
            }
            int count = (blurRadius * 2 + 1) * (blurRadius * 2 + 1) * pixelByteCount;
            imageData[i][j] = sum / count;
        }
    }

    // 对数据矩阵进行小波逆变换
    haarInverseWavelet2D(imageData);

    // 将数据矩阵转换回图像数据
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < imageWidthBytes; j++) {
            int index = i * imageWidthBytes + j;
            unsigned char value = static_cast<unsigned char>(std::round(imageData[i][j]));
            image[index] = value;
        }
    }
}