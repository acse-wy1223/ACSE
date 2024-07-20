/**
 * @file Slice.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the Slice class for extracting and saving 2D slices from 3D volume data.
 *
 * The Slice class includes methods for extracting YZ and XZ slices from a Volume object.
 * These methods allow the slices to be extracted and saved, facilitating the analysis
 * and visualization of specific cross-sections of the volume data.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_SLICE_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_SLICE_H

#include "Volume.h"

/**
 * @class Slice
 * @brief Class for extracting 2D slices from a 3D volume.
 *
 * Provides functionality to extract specific 2D slices (YZ or XZ plane) from
 * a given 3D volume and save them as 2D images. The class supports extracting slices
 * at a given coordinate along the width or height of the volume.
 */
class Slice {
public:
 
   /**
     * @brief Default constructor for the Slice class.
     */
	Slice() {};

	/**
     * @brief Default destructor for the Slice class.
     */
	~Slice() {};

	/**
     * @brief Extracts a YZ slice from the volume at a given X coordinate and saves it as an image.
     * @param volume The volume to extract the slice from.
     * @param x The X coordinate at which to extract the slice.
     * @param outputPath The file path where the slice image will be saved.
     */
    void dosliceYZ(const Volume& volume, int x, const std::string& outputPath);

	/**
     * @brief Static method to extract a YZ slice from the volume at a given X coordinate.
     * @param volume The volume to extract the slice from.
     * @param x The X coordinate at which to extract the slice.
     * @param outputPath The file path where the slice image will be saved.
     */
	static void sliceYZ(const Volume& volume, int x, const std::string& outputPath) {
		
		int height = volume.getHeight();
		int width = volume.getWidth();
		int slices = volume.getSlices();
		std::vector<unsigned char*>  data = volume.getData();

		if (x < 1 || x > width) {
			std::cerr << "Invalid x coordinate for slicing." << std::endl;
			return;
		}
		x -= 1; // Adjust for 0-based indexing

		unsigned char* slice = new unsigned char[height * slices * 3];
		for (int s = 0; s < slices; ++s) {
			for (int y = 0; y < height; ++y) {
				for (int c = 0; c < 3; ++c) {
					slice[(y * slices + s) * 3 + c] = data[s][(y * width + x) * 3 + c];
				}
			}
		}
		//stbi_write_png(outputPath.c_str(), slices, height, 3, slice, slices * 3);
		delete[] slice;
	}

	/**
     * @brief Extracts an XZ slice from the volume at a given Y coordinate and saves it as an image.
     * @param volume The volume to extract the slice from.
     * @param y The Y coordinate at which to extract the slice.
     * @param outputPath The file path where the slice image will be saved.
     */
	void dosliceXZ(const Volume& volume, int y, const std::string& outputPath);

	/**
     * @brief Static method to extract an XZ slice from the volume at a given Y coordinate.
     * @param volume The volume to extract the slice from.
     * @param y The Y coordinate at which to extract the slice.
     * @param outputPath The file path where the slice image will be saved.
     */
	static void sliceXZ(const Volume& volume, int y, const std::string& outputPath) {
		int height = volume.getHeight();
		int width = volume.getWidth();
		int slices = volume.getSlices();
		std::vector<unsigned char*>  data = volume.getData();

		if (y < 1 || y > height) {
			std::cerr << "Invalid y coordinate for slicing." << std::endl;
			return;
		}
		y -= 1; // Adjust for 0-based indexing

		unsigned char* slice = new unsigned char[width * slices * 3];
		for (int s = 0; s < slices; ++s) {
			for (int x = 0; x < width; ++x) {
				for (int c = 0; c < 3; ++c) {
					slice[(s * width + x) * 3 + c] = data[s][(y * width + x) * 3 + c];
				}
			}
		}
		//stbi_write_png(outputPath.c_str(), width, slices, 3, slice, width * 3);
		delete[] slice;
	}
};


#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_SLICE_H
