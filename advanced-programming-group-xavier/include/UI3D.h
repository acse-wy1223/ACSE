/**
 * @file UI3D.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Defines the UI3D class for the 3D user interface of the application.
 *
 * This file contains the definition of the UI3D class, which manages the user interface for
 * 3D visualization and processing options. It allows users to choose different operations like
 * filtering, projection, and slicing on 3D volume data.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI3D_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI3D_H

#include "Helpers.h"
#include "Volume.h"
#include "GaussianFilter3D.h"
#include "MedianFilter3D.h"
#include "Projection.h"
#include "Slice.h"

using namespace helpers;

/**
 * @class UI3D
 * @brief User interface class for 3D visualization and processing options.
 *
 * UI3D class provides a menu-driven interface for 3D operations such as filtering,
 * projection, and slicing. It interacts with the user to obtain input and output paths,
 * filter parameters, and projection or slice specifications.
 */
class UI3D {
public:

   /**
     * @brief Starts the UI loop for 3D operations.
     * @return Returns the user's selection.
     */
	int run() { 
		int opt = mainMenu(); 
		return opt;
	}
private:
	std::string inputdir; ///< Directory path for input volume data.
    std::string outputdir; ///< Directory path for saving output data.

    /**
     * @brief Displays the main menu for 3D operations and handles user interaction.
     * @return Returns the user's selection.
     */
	int mainMenu();
	int enterInputPath(Volume& volume, std::string& path);
	std::string enterOutputPath(std::string& path);
	int filterMenu3D(Volume& volume, std::string& inputdir, std::string& outputdir);
	int gaussianFilterSize(int& filterSize, double& sigma);
	int gaussianSigma(int& filterSize, double& sigma);
	int medianFilterSize(int& filterSize);
	int operationMenu3D(Volume& volume, std::string& inputdir, std::string& outputdir);
	int projectionMenu(Volume& volume, std::string& inputdir, std::string& outputdir);
	int startSlice(int start, int end, int default_start, int default_end);
	int endSlice(int start, int end, int default_start, int default_end);
	int sliceMenu(Volume& volume, std::string& inputdir, std::string& outputdir);
	int sliceIndex(int default_index);
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI3D_H
