/**
 * @file Brightness.h
 * @author Jing-Han Huang (edsml-jh123)
 * @brief Provides the Brightness class for adjusting the brightness of images.
 * 
 * This file includes the declaration of the Brightness class, which inherits from Filter2D.
 * It is designed to adjust the brightness of an image by modifying the pixel values across all channels,
 * except for the alpha channel in RGBA images.
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
#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include "Image.h"
#include "Filter2D.h"

/**
 * @class Brightness
 * @brief Implements brightness adjustment for images.
 * 
 * The Brightness class is derived from Filter2D and allows for the brightness adjustment of an Image object.
 * It modifies the brightness by adding a specified value to the pixel values of the image.
 */
class Brightness : Filter2D {
public:
    /**
     * @brief Constructs a new Brightness filter object with a specified brightness adjustment value.
     * 
     * @param brightnessValue The brightness adjustment value. Positive values increase brightness,
     * while negative values decrease it.
     */
    Brightness(int brightnessValue);
    /**
     * @brief Applies brightness adjustment to the given image.
     * 
     * Modifies the given Image object by adjusting its brightness. The method iterates over each pixel,
     * adjusting the brightness by adding the specified brightness value to the pixel values, while ensuring
     * that the resulting value remains within the valid range of [0, 255].
     * 
     * @param image Reference to the Image object to be processed.
     */
    void apply(Image& image);

private:
    int brightnessValue;
};

#endif // BRIGHTNESS_H