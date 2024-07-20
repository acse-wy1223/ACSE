/**
 * @file EdgeDetection.h
 * @author Yongwen Chen (acse-yc3321), Ning Guan (edsml-ng323)
 * @brief Provides the EdgeDetection class for applying edge detection algorithms on images.
 * 
 * This file includes the definition of the EdgeDetection class, which inherits from Filter2D.
 * It supports various edge detection algorithms such as Sobel, Prewitt, Scharr, and Roberts. The class
 * allows for the flexible application of these algorithms to an Image object to highlight edges and contours.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_EdgeDetection_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_EdgeDetection_H

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "Image.h"
#include "Filter2D.h"

/**
 * @enum EdgeDetectionAlgorithm
 * @brief Enumerates the supported edge detection algorithms.
 */
enum class EdgeDetectionAlgorithm {
    Sobel,
    Prewitt,
    Scharr,
    Roberts
};

/**
 * @class EdgeDetection
 * @brief Implements edge detection algorithms for images.
 * 
 * The EdgeDetection class, derived from Filter2D, is designed to apply various edge detection algorithms
 * to an Image object. It highlights the edges and contours within images by employing different convolution kernels.
 */
class EdgeDetection : Filter2D {
public:
    /**
     * @brief Constructs an EdgeDetection object with a specified algorithm.
     * 
     * @param algorithm The edge detection algorithm to be applied. Defaults to Sobel if not specified.
     */
    EdgeDetection(EdgeDetectionAlgorithm algorithm = EdgeDetectionAlgorithm::Sobel);
    /**
     * @brief Applies the selected edge detection algorithm to an image.
     * 
     * Modifies the given Image object by applying the edge detection algorithm specified at construction.
     * The function adjusts image pixels to highlight edges and contours based on the chosen algorithm.
     * 
     * @param image Reference to the Image object to be processed.
     */
    void apply(Image& image);

private:
    int width;
    int height;
    std::vector<unsigned char> img_data;
    EdgeDetectionAlgorithm algorithm;

    // Sobel operators
    const int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Prewitt operators
    const int prewitt_x[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    const int prewitt_y[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    // Scharr operators
    const int scharr_x[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
    const int scharr_y[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};

    // Roberts’ Cross operators
    const int roberts_x[2][2] = {{1, 0}, {0, -1}};
    const int roberts_y[2][2] = {{0, 1}, {-1, 0}};

    /**
     * @brief Applies a convolution kernel to the image for edge detection.
     * 
     * This helper method applies the specified convolution kernels to the image data for edge detection.
     * It supports both 3x3 kernels (Sobel, Prewitt, Scharr) and the special case of the 2x2 Roberts kernel.
     * 
     * @param image The Image object to be processed.
     * @param kernel_x The x-direction kernel for edge detection.
     * @param kernel_y The y-direction kernel for edge detection.
     * @param kernelSize The size of the kernel (3 for 3x3 kernels, 2 for Roberts).
     */
    void applyKernel(Image& image, const int kernel_x[][3], const int kernel_y[][3], int kernelSize);

};

#endif
