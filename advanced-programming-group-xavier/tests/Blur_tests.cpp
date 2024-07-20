#include <string>
#include "Blur_tests.h"
#include "../include/Kernel.h"
#include "../include/Blur.h"
#include "../include/Image.h"

void testMedianBlurring3by3() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 1;
    Image I(imgFilePath, h, w, c);

    // setup kernel.
    int kernelSize = 3;
    std::string blurType = "median";
    Kernel K(kernelSize, blurType);

    // apply blur.
    Blur B(K);
    B.apply(I);

//     1st channel (R) 3x3:
//                       | 0  0  0   0  0 |
//     [18 24 29]        | 0 [18 24 29] 0 |        [0  22  0]
//     [22 22 24]  --->  | 0 [22 22 24] 0 | --->   [18 22 17]
//     [18 17 17]        | 0 [18 17 17] 0 |        [0  17  0]
//                       | 0  0  0   0  0 |
    int k = 0;
    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            actual.emplace_back(I.getPixel(i, j, k));
        }
    }
    std::vector<int> desired{0, 22, 0, 18, 22, 17, 0, 17, 0};
    if (actual != desired) {
        throw std::runtime_error("Median blurring 3x3 incorrect.");
    }
}

void testBoxBlurring3by3() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 1;
    Image I(imgFilePath, h, w, c);

    // setup kernel
    int kernelSize = 3;
    std::string blurType = "box";
    Kernel K(kernelSize, blurType);

    // apply blur.
    Blur B(K);
    B.apply(I);

//                       | 0  0   0  0  0 |
//     [18 24 29]        | 0 [18 24 29] 0 |      [9.5  15.4 11.0]      [9  15 11]
//     [22 22 24]  --->  | 0 [22 22 24] 0 | ---> [13.4 21.2 14.7] ---> [13 21 14]
//     [18 17 17]        | 0 [18 17 17] 0 |      [8.7  13.3  8.9]      [8  13  8]
//                       | 0  0   0  0  0 |

    int k = 0;
    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            actual.emplace_back(I.getPixel(i, j, k));
        }
    }
    std::vector<int> desired{9, 15, 11, 13, 21, 14, 8, 13, 8};
    if (actual != desired) {
        throw std::runtime_error("box blurring 3x3 incorrect.");
    }
}

void testGaussianBlurring3by3() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 1;
    Image I(imgFilePath, h, w, c);

    // setup kernel
    int kernelSize = 3;
    std::string blurType = "gaussian";
    double sigma = 1.0;
    Kernel K(kernelSize, blurType, sigma);

    // apply blur.
    Blur B(K);
    B.apply(I);

    //  normalised 3x3 gaussian kernel:
    // | 0.0751 0.1238 0.0751 |
    // | 0.1238 0.2041 0.1238 |
    // | 0.0751 0.1238 0.0751 |

//                       | 0  0   0  0  0 |
//     [18 24 29]        | 0 [18 24 29] 0 |       [11.0244 16.9006 13.5181]      [11 16 13]
//     [22 22 24]  --->  | 0 [22 22 24] 0 | --->  [14.7544 21.4255 16.4012] ---> [14 21 16]
//     [18 17 17]        | 0 [18 17 17] 0 |       [16.4012 10.1576 13.9852]      [10 13 10]
//                       | 0  0   0  0  0 |

    int k = 0;
    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            actual.emplace_back(I.getPixel(i, j, k));
        }
    }
    std::vector<int> desired{11, 16, 13, 14, 21, 16, 10, 13, 10};
    if (actual != desired) {
        throw std::runtime_error("gaussian blurring 3x3 incorrect.");
    }
}