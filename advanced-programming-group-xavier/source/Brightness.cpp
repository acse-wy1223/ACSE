/**
 * @file Brightness.cpp
 * @author Jing-Han Huang (edsml-jh123)
 * @brief Implements the Brightness class for adjusting the brightness of images.
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
#include "../include/Brightness.h"

Brightness::Brightness(int brightnessValue) : Filter2D(), brightnessValue(brightnessValue) {}

void Brightness::apply(Image& image) {
    int height = image.getHeight(), width = image.getWidth(), channels = image.getChannels();
    // iterate through each pixel and adjust brightness.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (channels == 4) {
                for (int k = 0; k < channels - 1; ++k) {
                      int newValue = std::max(0, std::min(255, image.getPixel(i, j, k) + brightnessValue));
                      image.setPixel(i, j, k, newValue);
                }
            } else {
                for (int k = 0; k < channels; ++k) {
                    int newValue = std::max(0, std::min(255, image.getPixel(i, j, k) + brightnessValue));
                    image.setPixel(i, j, k, newValue);
                }
            }
        }
    }
}