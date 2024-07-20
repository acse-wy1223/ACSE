#include <iostream>
#include "Image_tests.h"
#include "Blur_tests.h"
#include "Kernel_tests.h"
#include "Utils_tests.h"
#include "HistogramEqualisation_test.h"
#include "Thresholding_test.h"
#include "Brightness_test.h"
#include "SaltAndPepperNoise_test.h"
#include "Grayscale_test.h"
#include "3D_test.h"
#include "EdgeDetection_test.h"

int main () {
    std::cerr << "----------------------" << std::endl;
    std::cerr << "\t*** TESTS ***" << std::endl;
    std::cerr << "----------------------\n" << std::endl;

    // Image.
    std::cerr << "Running Image() tests..." << std::endl;
    testImageCreateImageData();
    testImageAttributesReadOnlyConstructor();
    testImageAttributesReadWithDimsConstructor();
    testImageSetters();
    testImageFilePathDimsConstructor();
    testImageMethodCreatePaddedImageData();
    testImageDefaultConstructor();
    testImageCopyConstructor();
    testImageGetPixel();
    testImageGetPixelModDims();
    testImageSetPixel();
    testImageSetPixelModDims();

    // Blur.
    std::cerr << "Running Blur() tests..." << std::endl;
    testMedianBlurring3by3();
    testBoxBlurring3by3();
    testGaussianBlurring3by3();

    // Kernel.
    std::cerr << "Running Kernel() tests..." << std::endl;
    testKernelDefaultInstantiation();
    testKernelInstantiationNoSigma();
    testKernelInstantiationWithSigma();
    testKernelCalculatePaddingOddKernelSize();
    testKernelCalculatePaddingEvenKernelSize();
    testKernelGenerateMedianKernelOdd();
    testKernelGenerateMedianKernelEven();
    testKernelGenerateBoxKernelOdd();
    testKernelGenerateBoxKernelEven();
    testKernelGenerateGaussianKernelOdd();
    testKernelGenerateGaussianKernelEven();
    testKernelSettersAndGetters();
    testKernelApplyKernelMedian3by3();
    testKernelApplyKernelBox2by2();
    testKernelApplyKernelGaussian3by3();

    // Utils.
    std::cerr << "Running Utils() tests..." << std::endl;
    testGetVectorSumNotEmpty();
    testGetVectorSumNotEmptyFloats();
    testGetVectorSumEmpty();
    testGetVectorMidpointNotEmptyEven();
    testGetVectorMidpointNotEmptyOdd();
    testGetVectorMidpointEmpty();
    testQuicksort();
    testQuicksortSingleElementList();
    testMedianOfThree();

    // Histogram
    std::cerr << "Running HistogramEqualisation() tests..." << std::endl;
    //testHistogramEqualisationGray();
    testHistogramEqualisationHSV();
    testHistogramEqualisationHSL();

    // Thresholding
    std::cerr << "Running Thresholding() tests..." << std::endl;
    testThresholdingGray();
    testThresholdingHSV();
    testThresholdingHSL();

    // Brightness
    std::cerr << "Running Brightness() tests..." << std::endl;
    testBrightness();

    // Salt and Pepper
    std::cerr << "Running SaltAndPepperNoise() tests..." << std::endl;
    testSaltandPepperNoise();

    // Grayscale
    std::cerr << "Running Grayscale() tests..." << std::endl;
    testGrayscale();

    // Edge detection
    std::cerr << "Running EdgeDetection() tests..." << std::endl;
    testEdgeDetectionSobel();
    testEdgeDetectionScharr();
    testEdgeDetectionRoberts();
    testEdgeDetectionPrewitt();

    // 3D
    std::cerr << "Running 3D() tests..." << std::endl;
    Test3D test3D;
    test3D.testVolume();
    test3D.testGaussianFilter3D();
    test3D.testMedianFilter3D();
    test3D.testProjectionAIP();
    test3D.testProjectionMIP();
    test3D.testProjectionMinIP();
    test3D.testSliceYZ();
    test3D.testSliceXZ();
    return 0;
}