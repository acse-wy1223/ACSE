/**
 * @file Blur.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief Implements the Blur class used for applying a 2D blur filter to images using various kernels.
 * 
 * @copyright Xavier-Initialization (2024)
 * Daniel Seal (edsml-ds423)
 * Yongwen Chen (acse-yc3321)
 * Zeqi Li (acse-zl123)
 * Jing-Han Huang (edsml-jh123)
 * Wenbo Yu (acse-wy1223)
 * Ning Guan (edsml-ng323)
 * 
 */

#include <vector>
#include "../include/Blur.h"
#include "../include/Utils.h"

Blur::Blur(Kernel& kernel):
        Filter2D(), kernel(kernel) {
}

void Blur::apply(Image& image) {
    // get kernel info.
    Kernel K = getKernel();
    int pad = K.getPadding(), kSize = K.getKernelSize();
    KernelType kType = K.getKernelType();

    // create a copy of the image.
    Image imgCopy(image);
    imgCopy.createPaddedImageData(pad);

    // get copied image dims.
    int h = imgCopy.getHeight(),
            w = imgCopy.getWidth(),
            dc = imgCopy.getDesiredChannels();

    for (int i = pad; i < h-pad; i++) {
        for (int j = pad; j < w-pad; j++) {
            for (int k = 0; k < dc; k++) {
                std::vector<double>  kernelValues = Kernel::applyKernel(imgCopy, K, i, j, k);
                int value=0;
                switch (kType) {
                    case KernelType::Gaussian:
                        value = getVectorSum(kernelValues);
                        break;
                    case KernelType::Box:
                        value = getVectorSum(kernelValues) / (kSize * kSize);
                        break;
                    case KernelType::Median:
                        quickSort(kernelValues, 0, (kSize * kSize) - 1);
                        value = getVectorMidpoint(kernelValues);
                        break;
                    default:
                        // specified kernel type is not handled. Returning initialised value 0.0.
                        break;
                }
                // inplace update on original image's data on in memory.
                int hPixel = i-pad, wPixel = j-pad;
                image.setPixel(hPixel, wPixel, k, value);
            }
        }
    }
}

const Kernel& Blur::getKernel() const { return kernel; }