#include "Thresholding_test.h"
#include "string"
void testThresholdingGray() {
    std::string imgFilePath = "../Images/stinkbug.png";
    int h = 3, w = 3, c = 1;
    Image I(imgFilePath, h, w, c);

    int rawWidth = I.getWidth();
    int rawChannels = I.getChannels();

    Thresholding filter(105, true);
    filter.apply(I);

    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int idx = ((i * rawWidth) + j) * rawChannels + 0;
            actual.push_back(int(I.getDataPtr()[idx]));
        }
    }
    std::vector<int> desired{0, 0, 0, 255, 255, 255, 255, 255, 255};
    if (actual != desired) {
        throw std::runtime_error("Thresholding (gray) incorrect.");
    }
}

void testThresholdingHSV() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3;
    Image I(imgFilePath, h, w, c);
    int rawWidth = I.getWidth();
    int rawChannels = I.getChannels();

    Thresholding filter(60, true);
    filter.apply(I);

    for (int channel = 0; channel < c; ++channel) {
        std::vector<int> actual;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int idx = ((i * rawWidth) + j) * rawChannels + channel;
                actual.push_back(int(I.getDataPtr()[idx]));
            }
        }
        std::vector<int> desired;
        switch (channel) {
            case 0:
                desired = {255, 255, 255, 255, 255, 255, 255, 255, 255};
                break;
            case 1:
                desired = {255, 255, 255, 255, 255, 255, 255, 255, 255};
                break;
            case 2:
                desired = {255, 255, 255, 255, 255, 255, 255, 255, 255};
                break;
        }

        if (actual != desired) {
            throw std::runtime_error("Thresholding (HSV) incorrect for channel " + std::to_string(channel));
        }
    }
}

void testThresholdingHSL() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3;
    Image I(imgFilePath, h, w, c);

    Thresholding filter(60, false);
    filter.apply(I);

    int rawWidth = I.getWidth();
    int rawChannels = I.getChannels();

    for (int channel = 0; channel < c; ++channel) {
        std::vector<int> actual;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int idx = ((i * rawWidth) + j) * rawChannels + channel;
                actual.push_back(int(I.getDataPtr()[idx]));
            }
        }
        std::vector<int> desired;
        switch (channel) {
            case 0:
                desired = {0, 0, 0, 255, 0, 0, 0, 0, 0};
                break;
            case 1:
                desired = {0, 0, 0, 255, 0, 0, 0, 0, 0};
                break;
            case 2:
                desired = {0, 0, 0, 255, 0, 0, 0, 0, 0};
                break;
        }

        if (actual != desired) {
            throw std::runtime_error("Thresholding (HSL) incorrect for channel " + std::to_string(channel));
        }
    }
}