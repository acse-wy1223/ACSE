/**
 * @file Grayscale.h
 * @author Yongwen Chen (acse-yc3321)
 * @brief Provides the Grayscale class for converting images to grayscale.
 * 
 * This file includes the definition of the Grayscale class, which inherits from Filter2D. 
 * It is designed to apply a grayscale filter to an image, effectively transforming
 * a color image into grayscale by calculating luminance.
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
#ifndef GRAYSCALE_IMAGE_H
#define GRAYSCALE_IMAGE_H

#include "Image.h"
#include "Filter2D.h"
#include <cstring>
#include <iostream>

/**
 * @class Grayscale
 * @brief Implements image grayscaling.
 * 
 * The Grayscale class inherits from Filter2D and offers functionality to convert a given Image object 
 * into a grayscale image. It overrides the apply method to apply the grayscale filter.
 */
class Grayscale : Filter2D{
public:
    /**
     * @brief Constructs a new Grayscale object.
     * 
     * The default constructor.
     */
    Grayscale() = default;
    /**
     * @brief Applies the grayscale filter to a specified Image object.
     * 
     * Takes an Image object as a parameter and applies grayscaling to it. This method modifies the Image object
     * to reflect the conversion to grayscale, changing each pixel's color to its grayscale equivalent based on luminance.
     * 
     * @param image Reference to an Image object to be converted into grayscale.
     */
    void apply(Image& image);
};

#endif