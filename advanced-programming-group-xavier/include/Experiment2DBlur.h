/**
 * @file Experiment2DBlur.h
 * @author Daniel Seal (edsml-ds423)
 * @brief The Experiment2DBlur class is designed to run a series of experiments applying a 2D blur effect to an image with varying kernel sizes and types.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_EXPERIMENT2DBLUR_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_EXPERIMENT2DBLUR_H

#include "../include/Image.h"
#include "../include/Blur.h"
#include "../include/Kernel.h"

/**
 * @class Experiment2DBlur
 * @brief Class for running multiple blurring experiments on an image.
 *
 * This class handles the setup and execution of a series of blurring experiments.
 * Each experiment uses a different kernel size or type to apply a blur effect to a single image.
 */
class Experiment2DBlur {
public:
    /**
     * @brief Constructor that sets up the experiment parameters.
     * @param numExperiments Number of experiments to run.
     * @param imgFilePath Path to the image file.
     * @param initKernelSize Initial size of the kernel.
     * @param kernelType Type of the kernel to use for blurring.
     * @param kernelSizeJumps Increments by which to increase the kernel size after each experiment.
     * @param sigma Sigma value for Gaussian kernels.
     */
    Experiment2DBlur(int& numExperiments, std::string& imgFilePath,
                     int& initKernelSize, std::string& kernelType,
                     int& kernelSizeJumps, double& sigma);

    /** * @brief Runs the specified number of experiments with different kernel sizes. */
    void runNExperiments();

    /**
     * @brief Gets the number of experiments to be run.
     * @return The total number of experiments.
     */
    const int& getNumExperiments() const;

private:
    int initKernelSize, numExperiments, kernelSizeJumps; ///< Experiment parameters for kernel sizes and count.
    std::string imgFilePath, kernelType;                 ///< File path of the image and type of kernel to be used.
    double sigma;                                        ///< Sigma value for Gaussian kernels.

    /**
     * @brief Runs a single blurring experiment with a specified kernel size.
     * @param kernelSize_ The size of the kernel for this experiment.
     */
    void runExperiment(int& kernelSize_); // keep private as wrapped in runNExperiments(). Allows it to also access sourceMesh directly.
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_EXPERIMENT2DBLUR_H
