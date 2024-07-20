/**
 * @file SaltAndPepperNoise.h
 * @author Jing-Han Huang (edsml-jh123)
 * @brief Provides the SaltandPepperNoise class to add salt-and-pepper noise to images.
 * 
 * This file includes the declaration of the SaltandPepperNoise class, which inherits from Filter2D.
 * It is designed to introduce salt-and-pepper noise (random occurrences of black and white pixels) to an image,
 * based on a specified noise percentage.
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
#ifndef SALTANDPEPPERNOISE_H
#define SALTANDPEPPERNOISE_H

#include "Image.h"
#include "Filter2D.h"

/**
 * @class SaltAndPepperNoise
 * @brief Implements salt-and-pepper noise addition to images.
 * 
 * The SaltandPepperNoise class, derived from Filter2D, allows for the addition of salt-and-pepper noise
 * to an Image object. The noise is characterized by random black and white pixels scattered across the image,
 * with the degree of noise controlled by a percentage value.
 */
class SaltAndPepperNoise : Filter2D {
public:
    /**
     * @brief Constructs a new Salt and Pepper Noise object.
     * 
     * @param noisePercentage The percentage of the image to be affected by salt-and-pepper noise,
     * specified as a double. For example, 10 means 10% of the image pixels will be affected.
     */
    SaltAndPepperNoise(double noisePercentage);
    /**
     * @brief Applies salt-and-pepper noise to the given image.
     * 
     * Modifies the given Image object by adding salt-and-pepper noise to it, based on the specified noise
     * percentage at construction. The method randomly selects pixels to change to black or white, simulating the noise.
     * 
     * @param image Reference to the Image object to be processed.
     */
    void apply(Image& image);

private:
    double noisePercentage;
};

#endif // SALTANDPEPPERNOISE_H
