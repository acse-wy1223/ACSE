#include <string>
#include <stdexcept>
#include "../include/Image.h"

void testImageCreateImageData() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h=2, w=3, c=4;
    Image I(imgFilePath, h, w, c);
    std::vector<int> actualR, actualG, actualB, actualAlpha;
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            for (int k=0; k<c; k++) {
                switch (k) {
                    case 0:
                        actualR.emplace_back(I(i, j, k));
                        break;
                    case 1:
                        actualG.emplace_back(I(i, j, k));
                        break;
                    case 2:
                        actualB.emplace_back(I(i, j, k));
                        break;
                    case 3:
                        actualAlpha.emplace_back(I(i, j, k));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    // desired outputs.
    std::vector<int> desiredR{18, 24, 29, 22, 22, 24};
    std::vector<int> desiredG{20, 26, 31, 25, 25, 25};
    std::vector<int> desiredB{72, 80, 87, 78, 78, 81};
    std::vector<int> desiredAlpha{255, 255, 255, 255, 255, 255};

    // lets check our outputs.
    if ((actualR != desiredR) | (actualG != desiredG) | (actualB != desiredB) | (actualAlpha != desiredAlpha)) {
        throw std::runtime_error("channels not equivalent.");
    }
}

void testImageAttributesReadOnlyConstructor() {
    std::string imgFilePath = "../Images/gracehopper.png";
    Image I(imgFilePath);

    int desiredH = 600, desiredW = 512, desiredC = 4;

    // after reading in image, check that h, w and c are initialised correctly.
    if (!((I.getHeight() == I.getRawHeight()) && (I.getHeight() == desiredH) &&
          (I.getWidth() == I.getRawWidth()) && (I.getWidth() == desiredW) &&
          (I.getChannels() == I.getDesiredChannels()) && (I.getChannels() == desiredC))) {
        throw std::runtime_error("Image Constructor Read Only: attributes initialised incorrectly.");
    }
}

void testImageAttributesReadWithDimsConstructor() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h=3, w=4, c=1;
    Image I(imgFilePath, h, w, c);

    int desiredH = 3, desiredW = 4, desiredC = 1;
    int desiredRawH = 600, desiredRawW = 512, desiredRawC = 4;

    // after reading in image, check that h, w and c are initialised correctly.
    if (!((I.getHeight() == desiredH) || (I.getRawHeight() == desiredRawH) ||
          (I.getWidth() == desiredW) || (I.getRawWidth() == desiredRawW) ||
          (I.getChannels() == desiredRawC) || (I.getDesiredChannels() == desiredC))) {
        throw std::runtime_error("Image Constructor Read+Dims: attributes initialised incorrectly.");
    }
}

void testImageSetters() {
    Image I;
    int desiredH = 1, desiredW = 2, desiredC = 3, desiredDC=4;

    I.setHeight(desiredH);
    I.setWidth(desiredW);
    I.setChannels(desiredC);
    I.setDesiredChannels(desiredDC);

    if (I.getHeight() != desiredH || I.getWidth() != desiredW || I.getChannels() != desiredC || I.getDesiredChannels() != desiredDC) {
        throw std::runtime_error("Image Constructor Read+Dims: attributes initialised incorrectly.");
    }
}

void testImageMethodCreatePaddedImageData() {
    std::string imgFilePath = "../Images/gracehopper.png";

    int h=3, w=3, c=2, padding = 1;
    Image I(imgFilePath, h, w, c);
    Image ICopy(I); // need to create a copy to be able to overwrite the .data (copying makes it start empty).
    ICopy.createPaddedImageData(padding);

    std::vector<int> actualR, actualG;
    for (int i=0; i<(h+(2*padding)); i++) {
        for (int j=0; j<(w+(2*padding)); j++) {
            for (int k=0; k<c; k++) {
                switch (k) {
                    case 0:
                        actualR.emplace_back(ICopy(i, j, k));
                        break;
                    case 1:
                        actualG.emplace_back(ICopy(i, j, k));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    // desired outputs.
    std::vector<int> desiredR{0, 0, 0, 0, 0, 0, 18, 24, 29, 0, 0, 22, 22, 24, 0, 0, 18, 17, 17, 0, 0, 0, 0, 0, 0};
    std::vector<int> desiredG{0, 0, 0, 0, 0, 0, 20, 26, 31, 0, 0, 25, 25, 25, 0, 0, 20, 20, 19, 0, 0, 0, 0, 0, 0};

    if ((actualR != desiredR) || (actualG != desiredG)) {
        throw std::runtime_error("padded image incorrect.");
    }
}

void testImageFilePathDimsConstructor() {
    std::string imgFilePath = "../Images/gracehopper.png";
    int h=3, w=4, c=1;
    Image I(imgFilePath, h, w, c);
    if (I.getHeight() != 3 ||
        I.getWidth() != 4 ||
        I.getChannels() != 4 ||
        I.getDesiredChannels() != 1 ||
        I.getRawHeight() != 600 ||
        I.getRawWidth() != 512) {
        throw std::runtime_error("Image constructor with filepath & image dims incorrect.");
    }
}

void testImageDefaultConstructor() {
    Image I;
    if (I.getDataPtr() != nullptr ||
        I.getHeight() != 0 ||
        I.getWidth() != 0 ||
        I.getChannels() != 0 ||
        I.getDesiredChannels() != 0 ||
        I.getRawHeight() != 0 ||
        I.getRawWidth() != 0) {
        throw std::runtime_error("default Image constructor incorrect.");
    }
}

void testImageCopyConstructor() {
    std::string imgFilePath = "../Images/gracehopper.png";
    Image I(imgFilePath);
    Image ICopy(I); // make a copy.

    if (I.getHeight() != ICopy.getHeight() ||
        I.getWidth() != ICopy.getWidth() ||
        I.getChannels() != ICopy.getChannels() ||
        I.getDesiredChannels() != ICopy.getDesiredChannels() ||
        I.getRawHeight() != ICopy.getRawHeight() ||
        I.getRawWidth() != ICopy.getRawWidth()) {
        throw std::runtime_error("Image copy constructor incorrect.");
    }
}

void testImageGetPixel() {
//     [18 24 29]    [20 26 31]    [72 80 87]
//   R [22 22 24]  G [25 25 25]  B [78 78 81]
//     [18 17 17]    [20 20 19]    [72 73 74]
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    Image I(imgFilePath);

    // R --> (2, 1, 0) = 17
    // G --> (1, 2, 1) = 25
    // B --> (1, 1, 2) = 80
    int desiredPixel1 = 17, desiredPixel2 = 25, desiredPixel3 = 81;

    int h=2, w=1, c=0;
    int actualPixel1 = I.getPixel(h, w, c);
    int h2=1, w2=2, c2=1;
    int actualPixel2 = I.getPixel(h2, w2, c2);
    int h3=1, w3=2, c3=2;
    int actualPixel3 = I.getPixel(h3, w3, c3);

    if (actualPixel1 != desiredPixel1 ||
        actualPixel2 != desiredPixel2 ||
        actualPixel3 != desiredPixel3) {
        throw std::runtime_error("Image getPixel() error.");
    }
}

void testImageGetPixelModDims() {
    // Read in pixels:
    //    [18 24 29]    [20 26 31]    [72 80 87]
    //  R [22 22 24]  G [25 25 25]  B [78 78 81]
    //    [18 17 17]    [20 20 19]    [72 73 74]
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    int imgH=2, imgW=2, imgC=1;
    Image I(imgFilePath, imgH, imgW, imgC);

    // mod dims so that the .data only has access to:
    //    [18 24]
    //  R [22 22]  (R only).
    //    [18 17]

    // test cases:
    // R1 --> (0, 0, 0) = 18
    // R2 --> (1, 1) = 22
    int desiredPixel1 = 18, desiredPixel2 = 22;

    int h=0, w=0, c=0;
    int actualPixel1 = I.getPixel(h, w, c);
    int h2=1, w2=1, c2=0;
    int actualPixel2 = I.getPixel(h2, w2, c2);

    if (actualPixel1 != desiredPixel1 ||
        actualPixel2 != desiredPixel2) {
        throw std::runtime_error("Image getPixel() error when modding image dims.");
    }
}

void testImageSetPixel() {
//     [18 24 29]    [20 26 31]    [72 80 87]
//   R [22 22 24]  G [25 25 25]  B [78 78 81]
//     [18 17 17]    [20 20 19]    [72 73 74]
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    Image I(imgFilePath);

    // R --> (2, 1, 0) = 17 set to 1.
    // G --> (1, 2, 1) = 25 set to 2.
    // B --> (1, 1, 2) = 81 set to 3.
    int desiredPixel1 = 1, desiredPixel2 = 2, desiredPixel3 = 3;

    int h=2, w=1, c=0, val=1;
    I.setPixel(h, w, c, val);
    int actualPixel1 = I.getPixel(h, w, c);
    int h2=1, w2=2, c2=1, val2=2;
    I.setPixel(h2, w2, c2, val2);
    int actualPixel2 = I.getPixel(h2, w2, c2);
    int h3=1, w3=2, c3=2, val3=3;
    I.setPixel(h3, w3, c3, val3);
    int actualPixel3 = I.getPixel(h3, w3, c3);

    if (actualPixel1 != desiredPixel1 ||
        actualPixel2 != desiredPixel2 ||
        actualPixel3 != desiredPixel3) {
        throw std::runtime_error("Image setPixel() error.");
    }
}

void testImageSetPixelModDims() {
    // Read in pixels:
    //    [18 24 29]    [20 26 31]    [72 80 87]
    //  R [22 22 24]  G [25 25 25]  B [78 78 81]
    //    [18 17 17]    [20 20 19]    [72 73 74]
    std::string imgFilePath = "../tests/data/test_image_3channel.png";
    int imgH=2, imgW=2, imgC=1;
    Image I(imgFilePath, imgH, imgW, imgC);

    // mod dims so that the .data only has access to:
    //    [18 24]
    //  R [22 22]  (R only).
    //    [18 17]

    // test cases:
    // R1 --> (0, 0, 0) = 18 set to 1.
    // R2 --> (1, 1) = 22 set to 2.
    int desiredPixel1 = 1, desiredPixel2 = 2;

    int h=0, w=0, c=0, val=1;
    I.setPixel(h, w, c, val);
    int actualPixel1 = I.getPixel(h, w, c);
    int h2=1, w2=1, c2=0, val2=2;
    I.setPixel(h2, w2, c2, val2);
    int actualPixel2 = I.getPixel(h2, w2, c2);

    if (actualPixel1 != desiredPixel1 ||
        actualPixel2 != desiredPixel2) {
        throw std::runtime_error("Image setPixel() error when modding image dims.");
    }
}