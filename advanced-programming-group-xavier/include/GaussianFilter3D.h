/**
 * @file GaussianFilter3D.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Defines the GaussianFilter3D class for applying a 3D Gaussian filter on volumetric data.
 * 
 * The GaussianFilter3D class extends from Filter3D and implements the application of a
 * Gaussian blur to a 3-dimensional volume. This is commonly used for noise reduction and
 * smoothing of volumetric data.
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
#ifndef GAUSSIANFILTER3D_H
#define GAUSSIANFILTER3D_H
#include "Filter3D.h"

/**
 * @class GaussianFilter3D
 * @brief Implements a 3D Gaussian filter on volumetric data.
 *
 * This class provides functionality to apply a Gaussian filter to a Volume object.
 * It encapsulates the filter size and standard deviation (sigma) and uses them to
 * create a Gaussian kernel for the filtering process.
 */

class GaussianFilter3D: Filter3D {

private:
    int filterSize; ///< Size of the Gaussian kernel.
    double sigma;   ///< Standard deviation of the Gaussian kernel.

public:

     /**
     * @brief Default constructor that initializes a Gaussian filter with a kernel size of 3 and sigma of 1.0.
     */

    GaussianFilter3D();
    /**
     * @brief Constructor to initialize a Gaussian filter with a specified kernel size and sigma.
     * @param filterSize Size of the filter kernel.
     * @param sigma Standard deviation for the Gaussian distribution.
     */
    GaussianFilter3D(int filterSize, double sigma);

    /**
     * @brief Default destructor.
     */
    ~GaussianFilter3D();

     /**
     * @brief Applies the Gaussian filter to a given volume.
     * @param volume Reference to the Volume object to be processed.
     */

    void apply(Volume& volume) override;

    /**
     * @brief Outputs information about the filter settings to the standard output.
     */

    void info() override;

    /**
     * @brief Sets or updates the filter size and sigma of the Gaussian kernel.
     * @param filterSize New size of the filter kernel.
     * @param sigma New standard deviation for the Gaussian distribution.
     */

    void setFilter(int filterSize, double sigma);
};
#endif //GAUSSIANFILTER3D_H
