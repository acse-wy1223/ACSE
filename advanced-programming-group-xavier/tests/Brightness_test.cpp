#include <iostream>
#include "../include/Image.h"
#include "../include/Filter2D.h"
#include "../include/Brightness.h"
#include "Brightness_test.h"

void testBrightness() {
    std::string imgFilePath = "../tests/data/test_image_3channel.png";

    int h = 3, w = 3, c = 3;
    Image I(imgFilePath, h, w, c);

    int brightnessValue = -50;
    Brightness B(brightnessValue);
    // apply brightness.
    B.apply(I);

    // Read in pixels:
    //    [18 24 29]    [20 26 31]    [72 80 87]
    //  R [22 22 24]  G [25 25 25]  B [78 78 81]
    //    [18 17 17]    [20 20 19]    [72 73 74]

    // actual and desired
    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < c; ++k) {
                actual.emplace_back(I.getPixel(i, j, k));
            }
        }
    }

    std::vector<int> desired{0, 0, 22, 0, 0, 30, 0, 0, 37, 
                             0, 0, 28, 0, 0, 28, 0, 0, 31, 
                             0, 0, 22, 0, 0, 23, 0, 0, 24};

    // check if actual and desired are identical
    if (actual != desired) {
        throw std::runtime_error("Brightness values incorrect. Error.");
    }
}