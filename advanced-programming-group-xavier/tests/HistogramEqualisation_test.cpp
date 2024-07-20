#include "HistogramEqualisation_test.h"
#include "string"
void testHistogramEqualisationGray() {
    std::string imgFilePath = "../Images/stinkbug.png";
    int h = 3, w = 3, c = 1;
    Image I(imgFilePath, h, w, c);
    int rawWidth = I.getWidth();
    int rawChannels = I.getChannels();

    HistogramEqualisation filter(true);
    filter.apply(I);

    std::vector<int> actual;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int idx = ((i * rawWidth) + j) * rawChannels + 0;
            actual.push_back(int(I.getDataPtr()[idx]));
        }
    }
    std::vector<int> desired{0, 0, 0, 85, 85, 212, 212, 212, 255};
    if (actual != desired) {
        throw std::runtime_error("Histogram equalisation (gray) incorrect.");
    }
}

void testHistogramEqualisationHSV() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3;
    Image I(imgFilePath, h, w, c);

    HistogramEqualisation filter(true);
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
                desired = {7, 17, 37, 66, 67, 52, 52, 70, 17};
                break;
            case 1:
                desired = {7, 18, 40, 70, 69, 55, 55, 67, 16};
                break;
            case 2:
                desired = {28, 56, 113, 198, 226, 198, 198, 255, 85};
                break;
        }

        if (actual != desired) {
            throw std::runtime_error("Histogram equalisation (HSV) incorrect for channel " + std::to_string(channel));
        }
    }
}

void testHistogramEqualisationHSL() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h = 3, w = 3, c = 3;
    Image I(imgFilePath, h, w, c);

    HistogramEqualisation filter(false);
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
                desired = {11, 39, 127, 255, 211, 76, 76, 210, 19};
                break;
            case 1:
                desired = {12, 42, 130, 255, 211, 78, 78, 210, 18};
                break;
            case 2:
                desired = {45, 130, 212, 254, 242, 207, 207, 243, 94};
                break;
        }

        if (actual != desired) {
            throw std::runtime_error("Histogram equalisation (HSL) incorrect for channel " + std::to_string(channel));
        }
    }
}