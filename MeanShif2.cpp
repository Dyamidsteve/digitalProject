#include "MeanShif2.h"

typedef unsigned char byte;

// 计算颜色距离
double color_distance(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2)
{
    double dr = r1 - r2, dg = g1 - g2, db = b1 - b2;
    return std::sqrt(dr * dr + dg * dg + db * db);
}

// 计算空间距离
double spatial_distance(int x1, int y1, int x2, int y2)
{
    double dx = x1 - x2, dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

// 计算均值漂移向量函数
void mean_shift_vector(const std::vector<byte>& image, int rows, int cols, double x, double y, double& shift_x, double& shift_y, double spatial_radius, double color_radius)
{
    double sum_r = 0, sum_g = 0, sum_b = 0;
    double sum_x = 0, sum_y = 0, num_pixels = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double dist = spatial_distance(i, j, x, y);
            if (dist > spatial_radius)
                continue;
            byte r = image[(i * cols + j) * 3];
            byte g = image[(i * cols + j) * 3 + 1];
            byte b = image[(i * cols + j) * 3 + 2];
            double color_dist = color_distance(r, g, b, image[(int)(x)*cols * 3 + (int)(y) * 3], image[(int)(x)*cols * 3 + (int)(y) * 3 + 1], image[(int)(x)*cols * 3 + (int)(y) * 3 + 2]);
            if (color_dist > color_radius)
                continue;
            sum_r += r;
            sum_g += g;
            sum_b += b;
            sum_x += i;
            sum_y += j;
            num_pixels += 1;
        }
    }

    double new_x = sum_x / num_pixels;
    double new_y = sum_y / num_pixels;
    if (std::abs(new_x - x) < 1e-3 && std::abs(new_y - y) < 1e-3) {
        shift_x = 0;
        shift_y = 0;
    }
    else {
        shift_x = new_x - x;
        shift_y = new_y - y;
    }
}

// 计算均值漂移向量(迭代函数)
void mean_shift_iteration(const std::vector<byte>& image, int rows, int cols, double x, double y, double& new_x, double& new_y, double spatial_radius, double color_radius)
{
    double shift_x = 0, shift_y= 0;
    do {
        double shift_x = 0, shift_y = 0;
        mean_shift_vector(image, rows, cols, x, y, shift_x, shift_y, spatial_radius, color_radius);
        x += shift_x;
        y += shift_y;
    } while (std::sqrt(shift_x * shift_x + shift_y * shift_y) > 1e-3);
    new_x = x;
    new_y = y;
}

// 图像分割函数
void mean_shift_segmentation(std::vector<unsigned char>& image, int height, int width, int channel, int spatial_radius, int color_radius, std::vector<unsigned char>& result)
{
    std::vector<byte> image_data = image;

    // 初始化标记数组
    bool* is_processed = new bool[height * width];
    memset(is_processed, 0, height * width);

    // 分割图像
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (is_processed[i * width + j])
                continue;

            double x = i, y = j;
            double new_x, new_y;
            mean_shift_iteration(image_data, height, width, x, y, new_x, new_y, spatial_radius, color_radius);

            // 设置同一区域中的像素值
            byte r = image_data[(int)new_x * width * channel + (int)new_y * channel];
            byte g = image_data[(int)new_x * width * channel + (int)new_y * channel + 1];
            byte b = image_data[(int)new_x * width * channel + (int)new_y * channel + 2];
            for (int k = 0; k < height; ++k) {
                for (int l = 0; l < width; ++l) {
                    double dist = spatial_distance(k, l, new_x, new_y);
                    if (dist < spatial_radius) {
                        result[k * width * channel + l * channel] = r;
                        result[k * width * channel + l * channel + 1] = g;
                        result[k * width * channel + l * channel + 2] = b;
                        is_processed[k * width + l] = true;
                    }
                }
            }
            ++count;
            if (count > 500) // 设置最大迭代次数
                break;
        }
    }

    delete[] is_processed;
}