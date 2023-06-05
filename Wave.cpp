#include "Wave.h"

ShockFilter::ShockFilter(std::vector<unsigned char> image, int width, int height, double standard_deviation, int threshold, int filter_size, int num_iterations) :
    image_{ image }, width_{ width }, height_{ height }, standard_deviation_{ standard_deviation }, threshold_{ threshold }, filter_size_{ filter_size }, num_iterations_{ num_iterations } {
}

void ShockFilter::calculate_gradient_magnitude() {
    gradient_magnitude_.resize(width_ * height_);
    for (int i = 1; i < height_ - 1; i++) {
        for (int j = 1; j < width_ - 1; j++) {
            double dx = image_[3 * ((i + 1) * width_ + j)] - image_[3 * ((i - 1) * width_ + j)];
            double dy = image_[3 * (i * width_ + (j + 1))] - image_[3 * (i * width_ + (j - 1))];
            gradient_magnitude_[i * width_ + j] = std::sqrt(dx * dx + dy * dy);
        }
    }
}

double ShockFilter::calculate_standard_deviation() {
    double sum = 0.0, mean = 0.0, standard_deviation = 0.0;
    int num_pixels = (width_ - 2) * (height_ - 2);
    for (int i = 1; i < height_ - 1; i++) {
        for (int j = 1; j < width_ - 1; j++) {
            sum += gradient_magnitude_[i * width_ + j];
        }
    }
    mean = sum / num_pixels;

    for (int i = 1; i < height_ - 1; i++) {
        for (int j = 1; j < width_ - 1; j++) {
            standard_deviation += std::pow(gradient_magnitude_[i * width_ + j] - mean, 2);
        }
    }
    standard_deviation /= num_pixels;
    standard_deviation = std::sqrt(standard_deviation);

    return standard_deviation;
}

bool ShockFilter::is_edge_pixel(int i, int j) {
    return gradient_magnitude_[i * width_ + j] > (threshold_ * standard_deviation_);
}

void ShockFilter::calculate_shock_value() {
    shock_value_.resize(width_ * height_);
    for (int i = filter_size_; i < height_ - filter_size_; i++) {
        for (int j = filter_size_; j < width_ - filter_size_; j++) {
            double sum1 = 0.0, sum2 = 0.0;
            for (int n = -filter_size_; n <= filter_size_; n++) {
                for (int m = -filter_size_; m <= filter_size_; m++) {
                    if (n == 0 && m == 0) {
                        continue;
                    }
                    int index = (i + n) * width_ + (j + m);
                    if (is_edge_pixel(i + n, j + m)) {
                        sum1 += gradient_magnitude_[index];
                    }
                    else {
                        sum2 += gradient_magnitude_[index];
                    }
                }
            }
            double a = sum1 / (2 * filter_size_ + 1);
            double b = sum2 / (2 * filter_size_ + 1);
            double ratio = a / b;
            shock_value_[i * width_ + j] = std::tanh(ratio);
        }
    }
}

void ShockFilter::filter_image() {
    std::vector<unsigned char> result(image_.size());
    for (int k = 0; k < num_iterations_; k++) {
        for (int i = filter_size_; i < height_ - filter_size_; i++) {
            for (int j = filter_size_; j < width_ - filter_size_; j++) {
                double sum = 0.0;
                double denominator = 0.0;
                for (int n = -filter_size_; n <= filter_size_; n++) {
                    for (int m = -filter_size_; m <= filter_size_; m++) {
                        int index = (i + n) * width_ + (j + m);
                        double weight = std::exp(-shock_value_[index] * shock_value_[index]);
                        sum += weight * gradient_magnitude_[index] * image_[3 * index];
                        denominator += weight * gradient_magnitude_[index];
                    }
                }
                result[3 * (i * width_ + j)] = static_cast<unsigned char>(sum / denominator);

                sum = 0.0;
                denominator = 0.0;
                for (int n = -filter_size_; n <= filter_size_; n++) {
                    for (int m = -filter_size_; m <= filter_size_; m++) {
                        int index = (i + n) * width_ + (j + m);
                        double weight = std::exp(-shock_value_[index] * shock_value_[index]);
                        sum += weight * gradient_magnitude_[index] * image_[3 * index + 1];
                        denominator += weight * gradient_magnitude_[index];
                    }
                }
                result[3 * (i * width_ + j) + 1] = static_cast<unsigned char>(sum / denominator);

                sum = 0.0;
                denominator = 0.0;
                for (int n = -filter_size_; n <= filter_size_; n++) {
                    for (int m = -filter_size_; m <= filter_size_; m++) {
                        int index = (i + n) * width_ + (j + m);
                        double weight = std::exp(-shock_value_[index] * shock_value_[index]);
                        sum += weight * gradient_magnitude_[index] * image_[3 * index + 2];
                        denominator += weight * gradient_magnitude_[index];
                    }
                }
                result[3 * (i * width_ + j) + 2] = static_cast<unsigned char>(sum / denominator);
            }
        }
        image_ = result;
    }
}

void ShockFilter::filter() {
    calculate_gradient_magnitude();
    double stddev = calculate_standard_deviation();

    calculate_shock_value();

    filter_image();
}

std::vector<unsigned char> ShockFilter::get_result() {
    return image_;
}