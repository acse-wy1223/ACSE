/**
 * @file Projection.cpp
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
#include "../include/Projection.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/Helpers.h"
#include "../include/tqdm.hpp"

using namespace helpers;

void Projection::applyMIP(const Volume& volume, const std::string& outputPath, int startSlice, int endSlice) {
    int height = volume.getHeight();
    int width = volume.getWidth();
    int slices = volume.getSlices();

    if (startSlice == -1) {
        startSlice = 0;
    }else {
        startSlice -= 1;

    }
    if (endSlice == -1) {
        endSlice = slices - 1;
    } else {
        endSlice -= 1;
    }

	if (startSlice > endSlice || startSlice < 0 || startSlice >= slices || endSlice < 0 || endSlice >= slices) {
		std::cerr << "Invalid slice range." << std::endl;
		return;
	}
	//print("startSlice: " + std::to_string(startSlice));
	//print("endSlice: " + std::to_string(endSlice));
	std::vector<unsigned char*>  data = volume.getData();
	unsigned char* projection = new unsigned char[width * height * 3];
	std::fill_n(projection, width * height * 3, 0);
	//
	int slicesToProcess = endSlice - startSlice + 1;
	auto A = get_set(slicesToProcess);
	auto T = tq::tqdm(A);
	std::string times = "*";
	T.set_prefix("Applying MIP: ");
	int count = 0;
	int s = startSlice;
	//
	for (auto&& t : T)
	//for (int s = startSlice; s <= endSlice; ++s) 
	{
		std::string str1 = std::to_string(s + 1) + "/" + std::to_string(slicesToProcess);
		T << str1;
		//print("Processing slice " + std::to_string(s + 1));
		for (int i = 0; i < width * height * 3; ++i) {
			projection[i] = std::max(projection[i], data[s][i]);
		}
		++s;
		count++;
		//clearScreen();
	}
	std::cout << "count: " << std::to_string(count) << std::endl;
	//stbi_write_png(outputPath.c_str(), width, height, 3, projection, width * 3);
	volume.saveSlice(outputPath, projection, width, height);
	delete[] projection;
}

void Projection::applyMinIP(const Volume& volume, const std::string& outputPath, int startSlice, int endSlice) {
    int height = volume.getHeight();
    int width = volume.getWidth();
    int slices = volume.getSlices();

    if (startSlice == -1) {
        startSlice = 0;
    }else {
        startSlice -= 1;

    }
    if (endSlice == -1) {
        endSlice = slices - 1;
    } else {
        endSlice -= 1;
    }

	if (startSlice > endSlice || startSlice < 0 || startSlice >= slices || endSlice < 0 || endSlice >= slices) {
		std::cerr << "Invalid slice range." << std::endl;
		return;
	}
	std::vector<unsigned char*>  data = volume.getData();
	unsigned char* projection = new unsigned char[width * height * 3];
	std::fill_n(projection, width * height * 3, 255);
	//
	int slicesToProcess = endSlice - startSlice + 1;
	auto A = get_set(slicesToProcess);
	auto T = tq::tqdm(A);
	std::string times = "*";
	T.set_prefix("Applying MinIP: ");
	int count = 0;
	int s = startSlice;
	//
	for (auto&& t : T) 	//for (int s = startSlice; s <= endSlice; ++s) 
	//for (int s = startSlice; s <= endSlice; ++s) 
	{
		std::string str1 = std::to_string(s + 1) + "/" + std::to_string(slicesToProcess);
		T << str1;
		//print("Processing slice " + std::to_string(s + 1));
		for (int i = 0; i < width * height * 3; ++i) {
			projection[i] = std::min(projection[i], data[s][i]);
		}
		++s;
		count++;
		//clearScreen();
	}
	std::cout << "count: " << std::to_string(count) << std::endl;
	std::cout << "s:" << s << std::endl;
	stbi_write_png(outputPath.c_str(), width, height, 3, projection, width * 3);
	//volume.saveSlice(outputPath, projection, width, height);
	delete[] projection;
}

void Projection::applyAIP(const Volume& volume, const std::string& outputPath, int startSlice, int endSlice) {
	int height = volume.getHeight();
	int width = volume.getWidth();
	int slices = volume.getSlices();

    if (startSlice == -1) {
        startSlice = 0;
    }else {
        startSlice -= 1;

    }
	if (endSlice == -1) {
		endSlice = slices - 1;
	} else {
        endSlice -= 1;
    }
//    std::cout << "startSlice: " << startSlice << std::endl;
//    std::cout << "endSlice: " << endSlice << std::endl;
	if (startSlice > endSlice || startSlice < 0 || startSlice >= slices || endSlice < 0 || endSlice >= slices) {
		std::cerr << "Invalid slice range." << std::endl;
		return;
	}
	int slicesToProcess = endSlice - startSlice + 1;
	//print("sliceToProcess: " + std::to_string(slicesToProcess));

	std::vector<unsigned char*>  data = volume.getData();
	std::vector<int> sum(width * height * 3, 0);
	//
	auto A = get_set(slicesToProcess);
	auto T = tq::tqdm(A);
	std::string times = "*";
	T.set_prefix("Applying AIP: ");
	//int count = 0;
	int s = startSlice;
	//
	for (auto&& t : T) 	//for (int s = startSlice; s <= endSlice; ++s) 
	{
		std::string str1 = std::to_string(s + 1) + "/" + std::to_string(slicesToProcess);
		T << str1;
		//std::cout << std::to_string(s + 1) << "/" << slicesToProcess << std::endl;
		for (int i = 0; i < width * height * 3; ++i) {
			sum[i] += data[s][i];
		}
		//count++;
		++s;
		//clearScreen();
	}
	//std::cout << "count: " << std::to_string(count) << std::endl;

	unsigned char* projection = new unsigned char[width * height * 3];
	for (int i = 0; i < width * height * 3; ++i) {
		projection[i] = static_cast<unsigned char>(sum[i] / slicesToProcess);
	}
	stbi_write_png(outputPath.c_str(), width, height, 3, projection, width * 3);
	//volume.saveSlice(outputPath, projection, width, height);
	delete[] projection;
}
