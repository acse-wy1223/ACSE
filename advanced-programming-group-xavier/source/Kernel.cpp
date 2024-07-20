/**
 * @file Kernel.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief Implements the Kernel class used for instantiating 2D kernels and applying them on images.
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

#include <cmath>
#include "../include/Kernel.h"

Kernel::Kernel(): kernelSize(0), kernelType(KernelType::None), sigma(0.0), padding(0) {}

Kernel::Kernel(int& kernelSize, std::string& kernelType):
    kernelSize(kernelSize), kernelType(stringToKernelType(kernelType)), sigma(0.0) {
    calculatePadding();  // init padding.
    generateKernel();  // set kernel.
}

Kernel::Kernel(int& kernelSize, std::string& kernelType, double& sigma):
    kernelSize(kernelSize), kernelType(stringToKernelType(kernelType)), sigma(sigma) {
    calculatePadding();  // init padding.
    generateKernel();  // set kernel.
}

KernelType Kernel::stringToKernelType(std::string& str) {
    if (str == "gaussian") return KernelType::Gaussian;
    if (str == "box") return KernelType::Box;
    if (str == "median") return KernelType::Median;
    return KernelType::None;  // Default case (in case of incorrect string).
}

void Kernel::calculatePadding() {
    int pad = (kernelSize % 2 == 1) ? (kernelSize - 1) / 2 : kernelSize / 2;
    setPadding(pad);
}

void Kernel::generateKernel() {
    std::vector<double> kernel_(kernelSize * kernelSize, 1.0); // initialise with 1s to avoid redo for non-gaussian.
    int offset = 1 - (kernelSize % 2); // handle even kernels.
    switch (kernelType) {
        case KernelType::Gaussian: {
            double weight, totalWeight = 0.0;
            int idx = 0;
            for (int k = -padding + offset; k <= padding; k++) {
                for (int l = -padding + offset; l <= padding; l++) {
                    weight = std::exp(-(std::pow(k, 2) + std::pow(l, 2)) / (2 * std::pow(sigma, 2)));
                    totalWeight += weight;
                    kernel_[idx] = weight;  // overwrite the value.
                    idx++;
                }
            }
            // normalise the gaussian kernel.
            for (double &val : kernel_) {
                val /= totalWeight;
            }
            setKernel(kernel_);
            break;
        }
        case KernelType::Box:
        case KernelType::Median:
            // box and median, the kernel is set without modifications.
            setKernel(kernel_);
            break;
        default:
            // specified kernel type is not handled. Defaulting to an all 1 kernel.
            setKernel(kernel_);
            break;
    }
}

std::vector<double> Kernel::applyKernel(Image& image, Kernel& k, int& iPoint, int& jPoint, int& zPoint) {
    std::vector<double> kernel_ = k.getKernel();
    int kernelSize_ = k.getKernelSize();
    int offset = kernelSize_ / 2;
    std::vector<double> kernelValues;  // to store pixel*kernel
    kernelValues.reserve(kernelSize_*kernelSize_);
    for (int i = 0; i < kernelSize_; i++) {
        for (int j = 0; j < kernelSize_; j++) {
            int x = iPoint - offset + i + (kernelSize_ % 2 == 0);
            int y = jPoint - offset + j + (kernelSize_ % 2 == 0);
            kernelValues.emplace_back(kernel_[(i*kernelSize_)+j]*image(x, y, zPoint));
        }
    }
    return kernelValues;
}

void Kernel::setPadding(int& p) { padding = p; }
void Kernel::setKernel(std::vector<double>& k) { kernel = k; }
const std::vector<double>& Kernel::getKernel() const { return kernel; }
const int& Kernel::getKernelSize() const { return kernelSize; }
const int& Kernel::getPadding() const { return padding; }
const double& Kernel::getSigma() const { return sigma; }
const KernelType& Kernel::getKernelType() const { return kernelType; }