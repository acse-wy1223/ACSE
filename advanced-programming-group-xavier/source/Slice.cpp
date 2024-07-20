/**
 * @file Slice.cpp
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the Slice class for extracting and saving 2D slices from 3D volume data.
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
#include "../include/Slice.h"

void Slice::dosliceYZ(const Volume& volume, int x, const std::string& outputPath) {
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
	//int success = stbi_write_png(imagePathStr.c_str(), w, h, c, data, 0);
	volume.saveSlice(outputPath, slice, slices, height);
	delete[] slice;
}

void Slice::dosliceXZ(const Volume& volume, int y, const std::string& outputPath) {
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
	volume.saveSlice(outputPath, slice, width, slices); // w, h
	delete[] slice;
}