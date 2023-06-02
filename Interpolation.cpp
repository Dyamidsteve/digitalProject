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