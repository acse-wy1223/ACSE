/**
 * @file Grayscale.cpp
 * @author Yongwen Chen (acse-yc3321)
 * @brief Implements the Grayscale class for converting images to grayscale.
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
#include "../include/Grayscale.h"

void Grayscale::apply(Image& image) {
    unsigned char*& data = image.getDataPtr();
    int width = image.getWidth(), height = image.getHeight(), channel = image.getChannels(), channels = 1;
    // assign new memory for the single grayscale channel.
    size_t dataSize = width * height * channels;
    unsigned char* newData = new unsigned char[dataSize];
    if (channel == 1) {
        std::cout << "The image is already grayscale." << std::endl;
        return;
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = data[(y * width + x) * channel + 0];
            unsigned char g = data[(y * width + x) * channel + 1];
            unsigned char b = data[(y * width + x) * channel + 2];
            unsigned char gray = static_cast<unsigned char>(0.2126 * r + 0.7152 * g + 0.0722 * b);
            newData[y * width + x] = gray;
        }
    }
    // update image.
    int newC = 1;
    image.setDataPtr(newData);
    image.setChannels(newC);
    image.setDesiredChannels(newC);
}