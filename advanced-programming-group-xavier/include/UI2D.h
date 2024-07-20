/**
 * @file UI2D.h
 * @author Zeqi Li (acse-zl123)
 * @brief Provides the UI2D class for interacting with users.
 * 
 * This header file defines the UI2D class, which allows users to interact with
 * various image processing functionalities through a 2D user interface.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI2D_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI2D_H

#include <iostream>
#include <string>
#include "Image.h"
#include "Blur.h"
#include "Brightness.h"
#include "SaltAndPepperNoise.h"
#include "HistogramEqualisation.h"
#include "Grayscale.h"
#include "Thresholding.h"
#include "SaltAndPepperNoise.h"
#include "EdgeDetection.h"
#include "Kernel.h"

/**
 * @class UI2D
 * @brief User interface class for 2D visualization and processing options.
 *
 * UI2D class provides a menu-driven interface for 2D operations.
 */
class UI2D
{
private:
    /**
     * @brief Displays the main menu of the UI for 2D filters.
     * @return Return 0 if successful.
     */
    int mainMenu2D();
public:
    /**
     * @brief Default constructor for UI2D class.
     */
    UI2D() = default;
    /**
     * @brief Runs the 2D user interface for image processing.
     * @return Return 0 if successful.
     */
    int run();
};


#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI2D_H