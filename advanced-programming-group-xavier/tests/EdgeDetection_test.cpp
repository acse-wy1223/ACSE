#include <iostream>
#include <vector>
#include <stdexcept>
#include "../include/Image.h"
#include "../include/Filter2D.h"
#include "../include/Grayscale.h"
#include "../include/EdgeDetection.h"
#include "EdgeDetection_test.h"

void testEdgeDetectionSobel() {
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    int h = 3, w = 3, c = 3; 
    Image I(imgFilePath, h, w, c);
 
    Grayscale grayscale_filter;
    grayscale_filter.apply(I);
    EdgeDetection edge_detector_sobel_filter(EdgeDetectionAlgorithm::Sobel);
    edge_detector_sobel_filter.apply(I);

    unsigned char* data = I.getDataPtr();
    std::vector<int> actual(data, data + h*w);
    std::vector<int> desired{23, 29, 34, 28, 26, 28, 23, 23, 22};
 
    if (actual != desired) {
        throw std::runtime_error("testEdgeDetection_Sobel conversion resulted in unexpected values.");
    } 
}

void testEdgeDetectionScharr() {
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    int h = 3, w = 3, c = 3; 
    Image I(imgFilePath, h, w, c);

    Grayscale grayscale_filter;
    grayscale_filter.apply(I);
    EdgeDetection edge_detector_sobel_filter(EdgeDetectionAlgorithm::Scharr);
    edge_detector_sobel_filter.apply(I);

    unsigned char* data = I.getDataPtr();
    std::vector<int> actual(data, data + h*w);
    std::vector<int> desired{23, 29, 34, 28, 100, 28, 23, 23, 22};

    if (actual != desired) {
        throw std::runtime_error("testEdgeDetection_Scharr conversion resulted in unexpected values.");
    } 
}

void testEdgeDetectionRoberts() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3; 
    Image I(imgFilePath, h, w, c);

    Grayscale grayscale_filter;
    grayscale_filter.apply(I);
    EdgeDetection edge_detector_sobel_filter(EdgeDetectionAlgorithm::Roberts);
    edge_detector_sobel_filter.apply(I);

    unsigned char* data = I.getDataPtr();
    std::vector<int> actual(data, data + h*w);
    std::vector<int> desired{10, 2, 34, 5, 12, 29, 29, 30, 20 };

    if (actual != desired) {
        throw std::runtime_error("testEdgeDetection_Roberts conversion resulted in unexpected values.");
    }
}

void testEdgeDetectionPrewitt() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3; 
    Image I(imgFilePath, h, w, c);

    Grayscale grayscale_filter;
    grayscale_filter.apply(I);
    EdgeDetection edge_detector_sobel_filter(EdgeDetectionAlgorithm::Prewitt);
    edge_detector_sobel_filter.apply(I);

    unsigned char* data = I.getDataPtr();
    std::vector<int> actual(data, data + h*w);
    std::vector<int> desired{23, 29, 34, 35, 8, 29, 29, 30, 20 };
 
    if (actual != desired) {
        throw std::runtime_error("testEdgeDetection_Prewitt conversion resulted in unexpected values.");
    }
}