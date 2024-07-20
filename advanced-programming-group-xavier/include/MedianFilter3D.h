/**
 * @file MedianFilter3D.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Defines the MedianFilter3D class for applying a median filter to volumetric data.
 *
 * The MedianFilter3D class is derived from Filter3D and is designed to apply a median filtering
 * process to 3-dimensional volumetric data. Median filtering is a non-linear process useful in
 * reducing noise while preserving edges by selecting the median pixel value from the neighborhood
 * of each pixel in the volume.
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

#ifndef MEDIANFILTER3D_H
#define MEDIANFILTER3D_H
#include "Filter3D.h"

/**
 * @class MedianFilter3D
 * @brief Implements median filtering for volumetric data.
 *
 * MedianFilter3D extends the Filter3D class with methods specifically tailored for applying a median
 * filter to 3D volume data. The class handles the creation and application of a median filter kernel
 * of a given size.
 */
class MedianFilter3D: Filter3D {

private:
    int filterSize; ///< Size of the median filter kernel.
public:

   /**
     * @brief Default constructor that initializes a median filter with a default kernel size.
     */
    MedianFilter3D();

    /**
     * @brief Constructor that initializes a median filter with a specified kernel size.
     * @param filterSize The size of the filter kernel to be used.
     */
    MedianFilter3D(int filterSize);

    /**
     * @brief Destructor for the median filter.
     */
    ~MedianFilter3D();

    /**
     * @brief Applies the median filter to a given volume.
     * @param volume Reference to the Volume object to be processed.
     */
    void apply(Volume& volume) override;

    /**
     * @brief Outputs information about the filter settings to the standard output.
     */
    void info() override;

    /**
     * @brief Sets the filter size for the median filter.
     * @param filterSize The new size of the median filter kernel.
     */
    void setFilter(int filterSize);

    /**
     * @brief Initializes the histogram used in finding the median value and returns the median value.
     * @param data Reference to the volume data.
     * @param s The current slice index.
     * @param y The current row index.
     * @param x The current column index.
     * @param c The current channel index.
     * @param width The width of the volume.
     * @param height The height of the volume.
     * @param slices The number of slices in the volume.
     * @param histogram Reference to the histogram vector to be used for finding the median.
     * @return The median value as an unsigned char.
     */
    unsigned char initHistogramAndFindMedian(const std::vector<unsigned char*>& data, int s, int y, int x, int c, int width, int height, int slices, std::vector<int>& histogram);
};
#endif //MEDIANFILTER3D_H
