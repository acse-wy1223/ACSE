/**
 * @file Blur.h
 * @author Daniel Seal (edsml-ds423)
 * @brief The Blur class is used for applying a 2D blur filter to images using various kernels.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_BLUR_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_BLUR_H

#include "Filter2D.h"
#include "Image.h"
#include "Kernel.h"

/**
 * @class Blur
 * @brief Class derived from Filter2D to apply blur effects on images.
 *
 * The Blur class provides functionalities to apply a 2D blur filter using different types of kernels. It holds a Kernel object to be applied on Image objects.
 */
class Blur : public Filter2D {

public:
    /**
     * @brief Constructor that initializes the blur filter with a specified kernel.
     * @param kernel Reference to the kernel to be used for blurring.
     */
    explicit Blur(Kernel& kernel);

    /**
     * @brief Applies the blur filter to a 2D image.
     * @param image Reference to the image to apply the blur filter on.
     * @note This function overrides the pure virtual function in the base class Filter2D.
     */
    void apply(Image& image) override;

    /**
     * @brief Gets the kernel used by the blur filter.
     * @return A constant reference to the kernel.
     */
    const Kernel& getKernel() const;

private:
    Kernel kernel; ///< Kernel object that defines the type and size of the blur to be applied.
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_BLUR_H
