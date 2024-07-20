/**
 * @file Projection.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the Projection class for computing and applying projection techniques to 3D volume data.
 *
 * The Projection class provides methods to apply different types of projections like Maximum Intensity Projection (MIP),
 * Minimum Intensity Projection (MinIP), and Average Intensity Projection (AIP) to a given Volume. These methods facilitate
 * the visualization of 3D volume data in 2D form.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_PROJECTION_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_PROJECTION_H

#include "Volume.h"

/**
 * @class Projection
 * @brief Class to perform various projection operations on volume data.
 *
 * This class provides different projection operations that can be applied to Volume objects.
 * Projections include Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP),
 * and Average Intensity Projection (AIP). These projections help in visualizing volumetric data by
 * projecting the data into a two-dimensional image.
 */
class Projection {
public:

   /**
     * @brief Default constructor for the Projection class.
     */
	Projection() {};

	/**
     * @brief Default destructor for the Projection class.
     */
	~Projection() {};

	/**
     * @brief Applies Maximum Intensity Projection to a volume and saves the result to a specified path.
     * @param volume The volume to apply MIP to.
     * @param outputPath The path where the resulting image will be saved.
     * @param startSlice The starting slice index to consider for the projection.
     * @param endSlice The ending slice index to consider for the projection.
     */
	void applyMIP(const Volume& volume, const std::string& outputPath, int startSlice = -1, int endSlice = -1);

	/**
     * @brief Applies Minimum Intensity Projection to a volume and saves the result to a specified path.
     * @param volume The volume to apply MinIP to.
     * @param outputPath The path where the resulting image will be saved.
     * @param startSlice The starting slice index to consider for the projection.
     * @param endSlice The ending slice index to consider for the projection.
     */
	void applyMinIP(const Volume& volume, const std::string& outputPath, int startSlice = -1, int endSlice = -1);

	/**
     * @brief Applies Average Intensity Projection to a volume and saves the result to a specified path.
     * @param volume The volume to apply AIP to.
     * @param outputPath The path where the resulting image will be saved.
     * @param startSlice The starting slice index to consider for the projection.
     * @param endSlice The ending slice index to consider for the projection.
     */
	void applyAIP(const Volume& volume, const std::string& outputPath, int startSlice = -1, int endSlice = -1);
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_PROJECTION_H
