/**
 * @file Filter2D.h
 * @author Zeqi Li (acse-zl123)
 * @brief The Filter2D class is an abstract base class for 2D image filters.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER2D_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER2D_H

#include "Filter.h"
#include "Image.h"

/**
 * @class Filter2D
 * @brief An abstract base class for 2D image filters. Class derived from Filter.
 *
 * The Filter2D class is an abstract base class that defines the common interface for 2D image filters.
 * Derived classes must implement the apply() method to define the specific filtering operation.
 */
class Filter2D: Filter<Image> {

public:
    /** * @brief Default constructor. */
    Filter2D();
    ~Filter2D() override;
    /**
     * @brief Pure virtual function for applying the filter to an image.
     * @param image The image to apply the filter to.
     *
     * This is a pure virtual function that must be implemented by derived classes to define the specific filtering operation.
     */
    void apply(Image& image) override = 0; // abstract class.
};


#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER2D_H
