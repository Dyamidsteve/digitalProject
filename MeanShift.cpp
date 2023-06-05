#include "MeanShift.h"

vector<unsigned char> meanShift(const vector<unsigned char>& image, int width, int height, double spatialRadius, double colorRadius) {
    // ��ʼ�����ͼ��
    vector<unsigned char> result(image.size());
    copy(image.begin(), image.end(), result.begin());

    // ��������ƫ����
    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, -1, 0, 1 };

    // ���о�ֵƯ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // ��ȡ��ǰ���ص����ɫ
            int index = (y * width + x) * 3;
            double r = image[index], g = image[index + 1], b = image[index + 2];

            // ��ʼ���ܶ����ĺ�������
            double sumR = 0, sumG = 0, sumB = 0;
            double sumWeight = 0;
            int count = 0;

            // �����Ե�ǰ���ص�Ϊ���ĵ���������
            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i], ny = y + dy[i];
                if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                    // ��ȡ���ڵ����ɫ
                    int nIndex = (ny * width + nx) * 3;
                    double nr = image[nIndex], ng = image[nIndex + 1], nb = image[nIndex + 2];

                    // �������Ȩ�غͿռ�Ȩ��
                    double dR = nr - r, dG = ng - g, dB = nb - b;
                    double colorDist = dR * dR + dG * dG + dB * dB;
                    double spatialDist = (x - nx) * (x - nx) + (y - ny) * (y - ny);//sqrt((x - nx) * (x - nx) + (y - ny) * (y - ny));
                    double weight = exp(-colorDist / colorRadius - spatialDist / spatialRadius);

                    // �ۼ����������ɫֵ��Ȩ��
                    sumR += nr * weight;
                    sumG += ng * weight;
                    sumB += nb * weight;
                    sumWeight += weight;
                    count++;
                }
            }

            // �����ܶ����ĺ͸��µ�ǰ���ص����ɫֵ
            if (count > 0) {
                double newR = sumR / sumWeight, newG = sumG / sumWeight, newB = sumB / sumWeight;
                result[index] = static_cast<unsigned char>(std::min(255.0, std::max(0.0, newR)));
                result[index + 1] = static_cast<unsigned char>(std::min(255.0, std::max(0.0, newG)));
                result[index + 2] = static_cast<unsigned char>(std::min(255.0, std::max(0.0, newB)));
            }
        }
    }

    return result;
}