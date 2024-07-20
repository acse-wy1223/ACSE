// SaltandPepperNoiseTest.cpp
#include <iostream>
#include "../include/Image.h"
#include "../include/Filter2D.h"
#include "../include/SaltAndPepperNoise.h"
#include "SaltAndPepperNoise_test.h"

void testSaltandPepperNoise() {
    std::string imgFilePath = "../Images/gracehopper.png";

    int h = 30, w = 30, c = 4;
    Image I(imgFilePath, h, w, c);

    double noisePercentage = 30;
    SaltAndPepperNoise B(noisePercentage);

    B.apply(I);

    // count number of noise
    unsigned char*& data = I.getDataPtr();
    int count = 0;
    for (int i = 0; i < h * w * c; ++i) {
        // Skip the fourth channel if c equals 4
        if (c == 4 && (i + 1) % c == 0) {
            continue;
        }
        if (data[i] == 0 || data[i] == 255) {
            count++;
        }
    }

    // actual and desired
    double actual = (static_cast<double>(count) / (h * w * std::min(c, 3))) * 100.0;
    double desired = noisePercentage;

    // check if the difference between actual and desired > 5 percentage
    if (std::abs(actual - desired) > 5) {
        throw std::runtime_error("channels not equivalent.");
    }

}