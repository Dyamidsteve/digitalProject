#include "MeanShift.h"

// 计算像素距离的函数
float calculate_distance(const Pixel& p1, const Pixel& p2)
{
    float dr = p1.r - p2.r;
    float dg = p1.g - p2.g;
    float db = p1.b - p2.b;
    return dr * dr + dg * dg + db * db;
}

// 均值漂移函数
Pixel mean_shift(const std::vector<Pixel>& pixels, float bandwidth)
{
    Pixel center = pixels[0];
    while (true)
    {
        Pixel new_center{0,0,0};
        float total_weight = 0;
        for (const auto& pixel : pixels)
        {
            float distance = calculate_distance(pixel, center);
            if (distance < bandwidth * bandwidth)
            {
                // 加权平均计算新的中心点
                float weight = 1.0f - distance / (bandwidth * bandwidth);
                new_center.r += pixel.r * weight;
                new_center.g += pixel.g * weight;
                new_center.b += pixel.b * weight;
                total_weight += weight;
            }
        }
        if (total_weight > 0)
        {
            new_center.r /= total_weight;
            new_center.g /= total_weight;
            new_center.b /= total_weight;
        }
        if (calculate_distance(center, new_center) < 1)
        {
            break;
        }
        center = new_center;
    }
    return center;
}

// 分割图像函数
std::vector<std::vector<Pixel>> image_segmentation(const std::vector<unsigned char>& image, int width, int height, float bandwidth)
{
    std::vector<std::vector<Pixel>> segments;
    std::vector<std::vector<int>> visited(height, std::vector<int>(width, 0));
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (visited[i][j])
            {
                continue;
            }
            Pixel pixel{ image[i * width * 3 + j * 3 + 2], image[i * width * 3 + j * 3 + 1], image[i * width * 3 + j * 3] };
            Pixel center = mean_shift({ pixel }, bandwidth);
            std::vector<Pixel> segment{ pixel };
            visited[i][j] = segments.size() + 1;
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    int new_i = i + k;
                    int new_j = j + l;
                    if (new_i < 0 || new_i >= height || new_j < 0 || new_j >= width || visited[new_i][new_j])
                    {
                        continue;
                    }
                    Pixel new_pixel{ image[new_i * width * 3 + new_j * 3 + 2], image[new_i * width * 3 + new_j * 3 + 1], image[new_i * width * 3 + new_j * 3] };
                    if (calculate_distance(new_pixel, center) < bandwidth * bandwidth)
                    {
                        Pixel new_center = mean_shift({ new_pixel }, bandwidth);
                        if (calculate_distance(new_center, center) < bandwidth * bandwidth)
                        {
                            visited[new_i][new_j] = segments.size() + 1;
                            segment.push_back(new_pixel);
                        }
                    }
                }
            }
            segments.push_back(segment);
        }
    }
    return segments;
}