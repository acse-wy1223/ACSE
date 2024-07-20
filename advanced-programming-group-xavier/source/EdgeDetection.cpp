/**
 * @file EdgeDetection.cpp
 * @author Yongwen Chen (acse-yc3321), Ning Guan (edsml-ng323)
 * @brief Implements the EdgeDetection class for applying edge detection algorithms on images.
 * 
 * @copyright Xavier-Initialization (2024)
 * Daniel Seal (edsml-ds423)
 * Yongwen Chen (acse-yc3321)
 * Zeqi Li (acse-zl123)
 * Jing-Han Huang (edsml-jh123)
 * Wenbo Yu (acse-wy1223)
 * Ning Guan (edsml-ng323)
 * 
 */
#include "../include/EdgeDetection.h"

EdgeDetection::EdgeDetection(EdgeDetectionAlgorithm algorithm) : Filter2D(), algorithm(algorithm) {}

void EdgeDetection::apply(Image& image) {
    width = image.getWidth();
    height = image.getHeight();
    img_data.resize(width * height);
    img_data.assign(image.getDataPtr(), image.getDataPtr() + width * height);
    switch (algorithm) {
        case EdgeDetectionAlgorithm::Sobel:
            applyKernel(image, sobel_x, sobel_y, 3);
            break;
        case EdgeDetectionAlgorithm::Prewitt:
            applyKernel(image, prewitt_x, prewitt_y, 3);
            break;
        case EdgeDetectionAlgorithm::Scharr:
            applyKernel(image, scharr_x, scharr_y, 3);
            break;
        case EdgeDetectionAlgorithm::Roberts:
            // For Roberts, pass nullptrs and handle differently in applyKernel due to 2x2 kernel size
            applyKernel(image, nullptr, nullptr, 2);
            break;
    }
}

void EdgeDetection::applyKernel(Image& image, const int kernel_x[][3], const int kernel_y[][3], int kernelSize) {
    // Handle special case for Roberts, which uses a 2x2 kernel
    if (algorithm == EdgeDetectionAlgorithm::Roberts) {
        for (int y = 0; y < height - 1; ++y) {
            for (int x = 0; x < width - 1; ++x) {
                int gx = img_data[y * width + x] * roberts_x[0][0] + img_data[(y + 1) * width + (x + 1)] * roberts_x[1][1];
                int gy = img_data[y * width + (x + 1)] * roberts_y[0][1] + img_data[(y + 1) * width + x] * roberts_y[1][0];
                int magnitude = static_cast<int>(std::sqrt(gx * gx + gy * gy));
                magnitude = std::min(255, std::max(0, magnitude));
                image.getDataPtr()[y * width + x] = static_cast<unsigned char>(std::sqrt(gx * gx + gy * gy));
            }
        }
    } else {
        // Handle 3x3 kernels (Sobel, Prewitt, Scharr)
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int gx = 0, gy = 0;
                for (int j = -1; j <= 1; ++j) {
                    for (int i = -1; i <= 1; ++i) {
                        int pixel_value = img_data[(y + j) * width + (x + i)];
                        gx += pixel_value * kernel_x[j + 1][i + 1];
                        gy += pixel_value * kernel_y[j + 1][i + 1];
                    }
                }
                int magnitude = static_cast<int>(std::sqrt(gx * gx + gy * gy));
                magnitude = std::min(255, std::max(0, magnitude)); // Clamp to [0, 255]
                image.getDataPtr()[y * width + x] = static_cast<unsigned char>(magnitude);
            }
        }
    }
}