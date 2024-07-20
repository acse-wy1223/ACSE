/**
 * @file Kernel.h
 * @author Daniel Seal (edsml-ds423)
 * @brief The Kernel class is used for instantiating 2D kernels and applying them on images.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_H

#include <string>
#include <vector>
#include "../include/Image.h"

 /**
  * @enum KernelType
  * @brief Enumerates types of kernels that can be applied to images.
  */
enum class KernelType {
    None,
    Gaussian,
    Box,
    Median
};


/**
 * @class Kernel
 * @brief Class for instantiating 2D kernels and applying them on images.
 *
 * The Kernel class provides functionalities to create different types of image processing kernels such as Gaussian, Box, and Median, and apply them to Image objects.
 */
class Kernel {
public:
    /** * @brief Default constructor. */
    Kernel();

    /**
     * @brief Constructor that initializes a kernel with a specified size and type.
     * @param kernelSize Size of the kernel.
     * @param kernelType Type of the kernel as a string.
     */
    Kernel(int& kernelSize, std::string& kernelType);

    /**
     * @brief Constructor for a Gaussian kernel with a specified size, type, and sigma.
     * @param kernelSize Size of the kernel.
     * @param kernelType Type of the kernel as a string.
     * @param sigma Sigma value for Gaussian kernels.
     */
    Kernel(int& kernelSize, std::string& kernelType, double& sigma);

    /**
     * @brief Applies the kernel to a specific point in the image.
     * @param image Reference to the image.
     * @param k Kernel to be applied.
     * @param iPoint X-coordinate in the image.
     * @param jPoint Y-coordinate in the image.
     * @param zPoint Z-coordinate (channel) in the image.
     * @return A vector<double> containing the pixel and its neighbourhood values after applying the kernel.
     */
    static std::vector<double> applyKernel(Image& image, Kernel& k, int& iPoint, int& jPoint, int& zPoint);

    /**
     * @brief Converts a kernel type string to a KernelType enum.
     * @param str String representation of the kernel type.
     * @return The corresponding KernelType enum value.
     */
    static KernelType stringToKernelType(std::string& str);

    // setters.
    void setPadding(int& p);
    void setKernel(std::vector<double>& k);

    // getters.
    const std::vector<double>& getKernel() const;
    const int& getKernelSize() const;
    const int& getPadding() const;
    const double& getSigma() const;
    const KernelType& getKernelType() const;

private:
    int kernelSize;                                   ///< Size of the kernel.
    KernelType kernelType;                            ///< Type of the kernel.
    double sigma;                                     ///< Sigma value for Gaussian kernels.
    int padding;                                      ///< Padding added to the image when applying the kernel.
    std::vector<double> kernel;                       ///< Vector storing the kernel values.

    // private methods. Only executed on instantiation.
    /** * @brief Calculates the padding needed based on the kernel size. */
    void calculatePadding();
    /** * @brief Generates the kernel based on its type and parameters. */
    void generateKernel();
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_KERNEL_H