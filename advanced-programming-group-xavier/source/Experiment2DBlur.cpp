/**
 * @file Experiment2DBlur.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief Implements the Experiment2DBlur class designed to run a series of experiments applying a 2D blur effect to an image with varying kernel sizes and types.
 *
 * The purpose of the Experiment2DBlur class is to facilitate running multiple
 * blur filter tests on an image (for optimisation purposes).
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

#include <iostream>
#include <chrono>
#include "../include/Experiment2DBlur.h"

Experiment2DBlur::Experiment2DBlur(int& numExperiments, std::string& imgFilePath,
                                   int& initKernelSize, std::string& kernelType,
                                   int& kernelSizeJumps, double& sigma):
        numExperiments(numExperiments), imgFilePath(imgFilePath), initKernelSize(initKernelSize),
        kernelType(kernelType), kernelSizeJumps(kernelSizeJumps), sigma(sigma) {}

void Experiment2DBlur::runExperiment(int& kernelSize_) {
    // apply blurring.
    Image I(imgFilePath);
    int h = I.getHeight(), w = I.getWidth(), c = I.getChannels();
    Kernel K(kernelSize_, kernelType, sigma);
    Blur B(K);
    B.apply(I);

    // debug info.
    std::cout << "[Image Dims (h, w, c): (" << h << ", " << w << ", " << c
              << ") | Kernel: " << kernelSize_ << "x" << kernelSize_ << "| " << kernelType << " | ";
}

void Experiment2DBlur::runNExperiments() {
    int initKernelSize_ = initKernelSize;
    for (int i = 0; i<numExperiments; i++) {
        auto start = std::chrono::steady_clock::now();
        runExperiment(initKernelSize_);
        auto end = std::chrono::steady_clock::now();
        // debug info.
        std::cout << "t = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms]" << std::endl;
        initKernelSize_ += kernelSizeJumps;  // add 1 to the kernel spacing each time.
    }
}
