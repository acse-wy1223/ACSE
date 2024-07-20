/**
 * @file Thresholding.h
 * @author Zeqi Li (acse-zl123)
 * @brief Provides the Thresholding class for applying thresholding techniques on images.
 * 
 * This file includes the declaration of the Thresholding class, which inherits from Filter2D.
 * The class is designed to apply thresholding to an image, converting it to a binary image based
 * on a specified threshold value. It supports both grayscale and color images, with an option
 * to perform thresholding in the HSV/HSL color space for color images.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_THRESHOLDING_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_THRESHOLDING_H

#include "Image.h"
#include "Filter2D.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

/**
 * @class Thresholding
 * @brief Implements thresholding for image processing.
 * 
 * Thresholding converts an image to a binary image where pixels are turned either black or white
 * based on a threshold. For color images, thresholding can be applied to the intensity/lightness value
 * in the HSV/HSL color space.
 */
class Thresholding : Filter2D {
public:
    /**
     * @brief Constructs a Thresholding filter with a given threshold and color space option.
     * 
     * @param threshold The intensity threshold for the binary conversion. Pixels below this value will be black, above will be white.
     * @param isHSV Specifies whether to perform thresholding in the HSV color space or in the HSV color space for RGB images.
     */
    Thresholding(double threshold, bool isHSV);
    /**
     * @brief Applies thresholding to an image.
     * 
     * The method modifies the given Image object, applying thresholding to convert it into a binary image
     * based on the specified threshold value. For color images, the thresholding can be performed on the
     * value/lightness channel in the HSV/HSL color space if specified.
     * 
     * @param image Reference to the Image object to be processed.
     */
    void apply(Image& image);
private:
    int threshold;
    bool isHSV;
    void RGBToHSV(double r, double g, double b, double &h, double &s, double &v);
    void RGBToHSL(double r, double g, double b, double &h, double &s, double &l);
};


#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_THRESHOLDING_H