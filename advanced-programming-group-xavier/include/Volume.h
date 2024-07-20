/**
 * @file Volume.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the Volume class for managing 3D volume data.
 *
 * The Volume class provides functionalities for loading, saving, and manipulating
 * 3D volume data from a series of images. It supports operations like reloading
 * the original volume, applying filters, and generating or processing slices of the volume.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_VOLUME_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_VOLUME_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>
#include <algorithm>
#include "Image.h"

namespace fs = std::filesystem;

/**
 * @class Volume
 * @brief Manages loading, saving, and manipulating 3D volume data.
 *
 * This class encapsulates a 3D volume, represented as a series of 2D slices,
 * and provides methods for volume data manipulation, including loading and saving
 * the volume from/to disk, and accessing and modifying the volume data.
 */
class Volume {
private:
	int width, height, slices, channels; ///< Dimensions and number of channels of the volume.
    std::vector<unsigned char*> data; ///< Stores the volume data.
    std::vector<unsigned char*> originalData; ///< Stores the original volume data for reloading.

public:
	/**
     * @brief Default constructor.
     */
	Volume() : width(0), height(0), slices(0) {}

	/**
     * @brief Destructor, frees allocated memory.
     */
	~Volume();

	const int& getHeight()  const { return height; }
	const int& getWidth() const { return width; }
	const int& getSlices() const { return slices; }
	const int& getChannels() const { return channels; }
	const std::vector<unsigned char*>& getData() const { return data; }

	/**
     * @brief Sets new volume data, replacing the existing data.
     * @param newData The new data to be set.
     */
	void setData(const std::vector<unsigned char*>& newData);

	/**
     * @brief Loads volume data from a given directory.
     * @param directoryPath Path to the directory containing the volume data.
     * @return True if loading is successful, false otherwise.
     */
	bool loadVolume(const std::string& directoryPath);

	/**
     * @brief Saves the current volume to the specified directory.
     * @param directoryPath Path to the directory where the volume data will be saved.
     */
	void saveVolume(const std::string& directoryPath);

	/**
     * @brief Saves a single slice of the volume to a file.
     * @param outputPath Path to the file where the slice will be saved.
     * @param sliceData Data of the slice to be saved.
     * @param sliceWidth Width of the slice.
     * @param sliceHeight Height of the slice.
     */
	void saveSlice(const std::string& outputPath, const unsigned char* sliceData, int sliceWidth, int sliceHeight) const;

	/**
     * @brief Reloads the volume from the original data.
     */
	void reloadVolume();

	/**
     * @brief Clones data from a source to a destination vector.
     * @param source Source vector from which to clone data.
     * @param destination Destination vector where data will be cloned to.
     */
	void cloneData(const std::vector<unsigned char*>& source, std::vector<unsigned char*>& destination);

	/**
     * @brief Generates sample volume data for testing purposes.
     * @param directory Directory where sample data will be saved.
     * @param count Number of samples to generate.
     * @param width Width of the sample images.
     * @param height Height of the sample images.
     * @param seed Seed for random number generation.
     */
    static void generateSamples(const std::string& directory, int count, int width, int height, int seed=123);

	/**
     * @brief Reads and prints sample data from a directory.
     * @param directory Directory containing the sample data.
     */
    static void readAndPrintSamples(const std::string& directory);

	/**
     * @brief Prints the volume data to the standard output.
     */
    void printVolumeData() const;

	/**
     * @brief Retrieves the pixel data for the entire volume.
     * @return A vector containing the pixel data.
     */
    std::vector<unsigned char> getVolumePixelData() const;

	/**
     * @brief Loads and returns pixel data from an image file.
     * @param path Path to the image file.
     * @return A vector containing the pixel data.
     */
    std::vector<unsigned char> getImagePixelData(const std::string& path) const;
	
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_VOLUME_H
