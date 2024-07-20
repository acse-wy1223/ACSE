/**
 * @file Filter3D.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief The Filter3D class is an abstract base class for 3D volume filters.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER3D_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER3D_H

#include "Filter.h"
#include "Volume.h"

/**
 * @class Filter3D
 * @brief An abstract base class for 3D volume filters. Class derived from Filter.
 *
 * The Filter3D class is an abstract base class that defines the common interface for 3D volume filters.
 * Derived classes must implement the apply() and info() methods to define the specific filtering operation
 * and provide information about the filter, respectively.
 */
class Filter3D: Filter<Volume> {

public:
        /** * @brief Default constructor. */
       Filter3D ();
        /** * @brief Destructor. */
       ~Filter3D();

        /**
        * @brief Pure virtual function for applying the filter to a volume.
        * @param volume The volume to apply the filter to.
        *
        * This is a pure virtual function that must be implemented by derived classes to define the specific
        * filtering operation for 3D volumes.
        */
        void apply(Volume& volume) override = 0; // apply filter

        /**
        * @brief Pure virtual function for displaying information about the filter.
        *
        * This is a pure virtual function that must be implemented by derived classes to provide information
        * about the specific filter implementation.
        */
        virtual void info() = 0; // display information
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_FILTER3D_H