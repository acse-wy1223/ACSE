#include <string>
#include "Kernel_tests.h"
#include "../include/Kernel.h"
#include "../include/Image.h"

void testKernelDefaultInstantiation() {
    Kernel K;
    int desiredKernelSize = 0, actualKernelSize = K.getKernelSize();
    int desiredPadding = 0, actualPadding = K.getPadding();
    KernelType desiredKernelType = KernelType::None, actualKernelType = K.getKernelType();
    double desiredSigma = 0.0, actualSigma = K.getSigma();  // default sigma = 0.0.

    if (desiredKernelSize != actualKernelSize |
        desiredPadding != actualPadding |
        desiredKernelType != actualKernelType |
        desiredSigma != actualSigma) {
        throw std::runtime_error("Kernel instantiation without sigma incorrect.");
    }
}

void testKernelInstantiationNoSigma() {
    int kernelSize_ = 3;
    std::string kernelType_ = "test"; // str not handled so will default to None.
    Kernel K(kernelSize_, kernelType_);

    int desiredKernelSize = 3, actualKernelSize = K.getKernelSize();
    KernelType desiredKernelType = KernelType::None, actualKernelType = K.getKernelType();
    double desiredSigma = 0.0, actualSigma = K.getSigma(); // default sigma = 0.0.

    if (desiredKernelSize != actualKernelSize |
        desiredKernelType != actualKernelType |
        desiredSigma != actualSigma) {
        throw std::runtime_error("Kernel instantiation without sigma incorrect.");
    }
}

void testKernelInstantiationWithSigma() {
    int kernelSize_ = 5;
    std::string kernelType_ = "box";
    double sigma = 0.5;
    Kernel K(kernelSize_, kernelType_, sigma);
    KernelType desiredKernelType = KernelType::Box, actualKernelType = K.getKernelType();
    int desiredKernelSize = 5, actualKernelSize = K.getKernelSize();
    double desiredSigma = 0.5, actualSigma = K.getSigma();

    if (desiredKernelSize != actualKernelSize |
        desiredKernelType != actualKernelType |
        desiredSigma != actualSigma) {
        throw std::runtime_error("Kernel instantiation with sigma incorrect.");
    }
}


void testKernelCalculatePaddingOddKernelSize() {
    int kernelSize_ = 5;
    std::string kernelType_ = "test";
    Kernel K(kernelSize_, kernelType_);
    int desiredPadding = 2, actualPadding = K.getPadding();
    if (desiredPadding != actualPadding) {
        throw std::runtime_error("Kernel padding calculation for odd kernel size incorrect.");
    }
}

void testKernelCalculatePaddingEvenKernelSize() {
    int kernelSize_ = 4;
    std::string kernelType_ = "test";
    Kernel K(kernelSize_, kernelType_);
    int desiredPadding = 2, actualPadding = K.getPadding();
    if (desiredPadding != actualPadding) {
        throw std::runtime_error("Kernel padding calculation for even kernel size incorrect.");
    }
}

void testKernelGenerateMedianKernelOdd() {
    int kernelSize_ = 3;
    std::string kernelType_ = "median";
    Kernel K(kernelSize_, kernelType_);
    std::vector<double> desiredKernel(9, 1.0);
    if (desiredKernel != K.getKernel()) {
        throw std::runtime_error("Kernel's generate odd median kernel incorrect.");
    }
}

void testKernelGenerateMedianKernelEven() {
    int kernelSize_ = 2;
    std::string kernelType_ = "median";
    Kernel K(kernelSize_, kernelType_);
    std::vector<double> desiredKernel(4, 1.0);
    if (desiredKernel != K.getKernel()) {
        throw std::runtime_error("Kernel's generate even median kernel incorrect.");
    }
}

void testKernelGenerateBoxKernelOdd() {
    int kernelSize_ = 3;
    std::string kernelType_ = "box";
    Kernel K(kernelSize_, kernelType_);
    std::vector<double> desiredKernel(9, 1.0);
    if (desiredKernel != K.getKernel()) {
        throw std::runtime_error("Kernel's generate odd box kernel incorrect.");
    }
}

void testKernelGenerateBoxKernelEven() {
    int kernelSize_ = 2;
    std::string kernelType_ = "box";
    Kernel K(kernelSize_, kernelType_);
    std::vector<double> desiredKernel(4, 1.0);
    if (desiredKernel != K.getKernel()) {
        throw std::runtime_error("Kernel's generate even box kernel incorrect.");
    }
}

void testKernelGenerateGaussianKernelOdd() {
    int kernelSize_ = 3;
    std::string kernelType_ = "gaussian";
    double sigma = 1.0;
    Kernel K(kernelSize_, kernelType_, sigma);

    // | 0.0751 0.1238 0.0751 |
    // | 0.1238 0.2041 0.1238 |
    // | 0.0751 0.1238 0.0751 |
    std::vector<double> desiredKernel{0.0751136, 0.123841, 0.0751136,
                                      0.123841, 0.20418, 0.123841,
                                      0.0751136, 0.123841, 0.0751136};
    double tol = 0.0001;
    for (int i = 0; i<desiredKernel.size(); i++) {
        double delta = std::abs(desiredKernel[i] - K.getKernel()[i]);
        if (delta > tol) {
            // single element difference, vectors not equivalent.
            throw std::runtime_error("Kernel's generate odd gaussian kernel incorrect.");
        }
    }
}

void testKernelGenerateGaussianKernelEven() {
    int kernelSize_ = 2;
    std::string kernelType_ = "gaussian";
    double sigma = 1.0;
    Kernel K(kernelSize_, kernelType_, sigma);

    // | 0.387455 0.235003 |
    // | 0.235003 0.142536 |
    std::vector<double> desiredKernel{0.387456, 0.235004,
                                      0.235004, 0.142537};
    double tol = 0.0001;
    for (int i = 0; i<desiredKernel.size(); i++) {
        double delta = std::abs(desiredKernel[i] - K.getKernel()[i]);
        if (delta > tol) {
            // single element difference, vectors not equivalent.
            throw std::runtime_error("Kernel's generate even gaussian kernel incorrect.");
        }
    }
}

void testKernelSettersAndGetters() {
    int kernelSize_ = 2;
    std::string kernelType_ = "gaussian";
    Kernel K(kernelSize_, kernelType_);

    // kernel setter.
    std::vector<double> desiredKernel{1.0, 2.0, 3.0, 4.0};
    K.setKernel(desiredKernel);

    // padding setter.
    int desiredPadding = 99;
    K.setPadding(desiredPadding);

    if (desiredKernel != K.getKernel() || desiredPadding != K.getPadding()) {
        throw std::runtime_error("Kernel's kernel and padding setters error.");
    }
}

void testKernelApplyKernelMedian3by3() {
    std::string fp = "../tests/data/test_image_1channel.png";
    Image I(fp);

    int kernelSize_ = 3;
    std::string kernelType_ = "median";
    Kernel K(kernelSize_, kernelType_);

    // need to do center point to avoid padding error.
    int iPoint = 1, jPoint = 1, zPoint=0;
    std::vector<double> actualKernelValues = Kernel::applyKernel(I, K, iPoint, jPoint, zPoint);
    std::vector<double> desiredKernelValues{18, 24, 29, 22, 22, 24, 18, 17, 17};
    if (desiredKernelValues != actualKernelValues) {
        throw std::runtime_error("Kernel's apply 3x3 kernel (median) incorrect.");
    }
}

void testKernelApplyKernelBox2by2() {
    std::string fp = "../tests/data/test_image_1channel.png";
    Image I(fp);

    int kernelSize_ = 2;
    std::string kernelType_ = "box";
    Kernel K(kernelSize_, kernelType_);

    int iPoint = 0, jPoint = 1, zPoint=0;
    std::vector<double> actualKernelValues = Kernel::applyKernel(I, K, iPoint, jPoint, zPoint);
    std::vector<double> desiredKernelValues{24, 29, 22, 24};
    if (desiredKernelValues != actualKernelValues) {
        throw std::runtime_error("Kernel's apply 2x2 kernel (box) incorrect.");
    }
}

void testKernelApplyKernelGaussian3by3() {
    std::string fp = "../tests/data/test_image_1channel.png";
    Image I(fp);

    int kernelSize_ = 3;
    std::string kernelType_ = "gaussian";
    double sigma = 1.0;
    Kernel K(kernelSize_, kernelType_, sigma);

    // need to do center point to avoid padding error.
    int iPoint = 1, jPoint = 1, zPoint=0;
    std::vector<double> actualKernelValues = Kernel::applyKernel(I, K, iPoint, jPoint, zPoint);

    // | 0.0751136 0.123841 0.0751136 |   [18 24 29]     [1.35204 2.97219 2.17829]
    // | 0.123841  0.20418  0.123841  | * [22 22 24] --> [2.72451 4.49196 2.97219]
    // | 0.0751136 0.123841 0.0751136 |   [18 17 17]     [1.35204 2.1053  1.27693]

    std::vector<double> desiredKernelValues{1.35204, 2.97219, 2.17829,
                                            2.72451, 4.49196, 2.97219,
                                            1.35204, 2.1053,  1.27693};
    double tol = 0.0001;
    for (int i = 0; i<desiredKernelValues.size(); i++) {
        double delta = std::abs(desiredKernelValues[i] - actualKernelValues[i]);
        if (delta > tol) {
            // single element difference, vectors not equivalent.
            throw std::runtime_error("Kernel's generate even 3x3 gaussian kernel incorrect.");
        }
    }
}