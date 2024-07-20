/**
 * @file SaltAndPepperNoise.cpp
 * @author Jing-Han Huang (edsml-jh123)
 * @brief Implements the SaltandPepperNoise class for adding salt-and-pepper noise to images.
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
#include <cstdlib>
#include <ctime>
#include "../include/SaltAndPepperNoise.h"

SaltAndPepperNoise::SaltAndPepperNoise(double noisePercentage) : Filter2D(), noisePercentage(noisePercentage) {
    // Seed the random number generator
    std::srand(std::time(nullptr));
}

void SaltAndPepperNoise::apply(Image& image) {
    int height = image.getHeight();
    int width = image.getWidth();
    int channels = image.getChannels();
    unsigned char*& data = image.getDataPtr();

    int totalPixels = height * width;
    int numNoisePixels = static_cast<int>(totalPixels * noisePercentage / 100.0f);

    // Create a vector to hold all pixel coordinates
    std::vector<std::pair<int, int>> pixelCoords;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixelCoords.emplace_back(x, y);
        }
    }

    // Shuffle the pixel coordinates using Fisher-Yates algorithm
    for (int i = totalPixels - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(pixelCoords[i], pixelCoords[j]);
    }

    // Add noise to the shuffled pixel coordinates
    for (int i = 0; i < numNoisePixels; ++i) {
        // Generate random pixel coordinates
        int x = pixelCoords[i].first;
        int y = pixelCoords[i].second;

        // Set pixel value to either 0 (black) or 255 (white) randomly
        int newValue = (std::rand() % 2) * 255;

        // Update the pixel value in the image for each channel
        if (channels == 4) {
            for (int z = 0; z < channels - 1; ++z) {
                int idx = ((y * width + x) * channels) + z;
                data[idx] = newValue;
            }
        } else {
            for (int z = 0; z < channels; ++z) {
                int idx = ((y * width + x) * channels) + z;
                data[idx] = newValue;
            }
        }
    }
}