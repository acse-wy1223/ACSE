/**
 * @file Filter.h
 * @author Daniel Seal (edsml-ds423)
 * @brief The Filter class is an abstract base class for image/volume filters.
 *
 * This file defines the Filter class, which is an abstract base class for
 * different types of filters. It provides a common interface and foundational
 * functionality for various filtering operations. Derived classes must implement
 * the apply() method to define the specific filtering operation for the
 * corresponding data type.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER_H

/**
 * @class Filter
 * @brief Base class for different types of filters.
 *
 * The Filter class provides a common interface and foundational functionality for various filtering operations. It is intended
 * to be subclassed by more specific filter types which implement actual filtering algorithms.
*/

template <typename T>
class Filter {
public:
    /** * @brief default constructor. */
    Filter();
    /**
     * @brief Pure virtual function for applying the filter to input data.
     * @param data The input data (e.g., Image or Volume) to apply the filter to.
     *
     * This is a pure virtual function that must be implemented by derived classes
     * to define the specific filtering operation for the corresponding data type.
     */
    virtual void apply(T& data) = 0;

    /** * @brief Virtual destructor. */
    virtual ~Filter() = default;
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER_H
