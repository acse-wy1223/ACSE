#include <iostream>
#include <vector>
#include <stdexcept>
#include "../include/Image.h"
#include "../include/Filter2D.h"
#include "../include/Grayscale.h"
#include "Grayscale_test.h"

void testGrayscale() {
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    int h = 3, w = 3, c = 3; 
    Image I(imgFilePath, h, w, c);

    Grayscale grayscale;
    grayscale.apply(I);

    unsigned char* dataa = I.getDataPtr(); 

    std::vector<int> actual(dataa, dataa + h*w);

    std::vector<int> desired{23, 29, 34, 28, 28, 28, 23, 23, 22};
 
    if (actual != desired) {
        throw std::runtime_error("Grayscale conversion resulted in unexpected values.");
    }
}