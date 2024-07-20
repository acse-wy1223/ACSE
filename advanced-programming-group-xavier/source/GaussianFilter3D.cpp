/**
 * @file GaussianFilter3D.cpp
 * @author Wenbo Yu (acse-wy1223)
 * @brief Implement the GaussianFilter3D class for applying a 3D Gaussian filter on volumetric data.
 * 
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
#include "../include/GaussianFilter3D.h"
#include "../include/tqdm.hpp"
#include "../include/Helpers.h"

using namespace helpers;

GaussianFilter3D::GaussianFilter3D() {
    this->filterSize = 3;
    this->sigma = 1.0;
}

GaussianFilter3D::GaussianFilter3D(int filterSize, double sigma) {
    this->filterSize = filterSize;
    this->sigma = sigma;
}

GaussianFilter3D::~GaussianFilter3D() = default;

void GaussianFilter3D::apply(Volume& volume) {
	int offset = filterSize / 2;
	std::vector<unsigned char*> newData;
	std::vector<std::vector<std::vector<double>>> kernel(filterSize, std::vector<std::vector<double>>(filterSize, std::vector<double>(filterSize)));
	double sum = 0.0;

	// Generate Gaussian kernel
	for (int x = 0; x < filterSize; ++x) {
		for (int y = 0; y < filterSize; ++y) {
			for (int z = 0; z < filterSize; ++z) {
				int dx = x - offset;
				int dy = y - offset;
				int dz = z - offset;
				double value = exp(-(dx * dx + dy * dy + dz * dz) / (2 * sigma * sigma));
				kernel[x][y][z] = value;
				sum += value;
			}
		}
	}

	// Normalize the kernel
	for (int x = 0; x < filterSize; ++x) {
		for (int y = 0; y < filterSize; ++y) {
			for (int z = 0; z < filterSize; ++z) {
				kernel[x][y][z] /= sum;
			}
		}
	}
	int height = volume.getHeight();
	int width = volume.getWidth();
	int slices = volume.getSlices();
	std::vector<unsigned char*>  data = volume.getData();

	// Apply filter
	//
	auto A = get_set(slices);
	auto T = tq::tqdm(A);
	std::string times = "*";
	std::string str = "(" + std::to_string(this->filterSize) + times + std::to_string(this->filterSize) + times + std::to_string(this->filterSize) + ", "+std::to_string(this->sigma)+")";
	T.set_prefix("Applying GaussianFilter3D " + str + ":");
	int slice = 0;
	int count = 0;
	for (auto&& t : T)
	//for (int slice = 0; slice < slices; ++slice) 
	{
		std::string str1 = std::to_string(slice + 1) + "/" + std::to_string(slices);
		T << str1;
		//std::cout << "Filtering: " << slice << std::endl;
		//std::cout << slice + 1 << "/" << slices << std::endl;
		unsigned char* newSlice = new unsigned char[width * height * 3];
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				double newPixel[3] = { 0, 0, 0 };
				for (int dz = -offset; dz <= offset; ++dz) {
					for (int dy = -offset; dy <= offset; ++dy) {
						for (int dx = -offset; dx <= offset; ++dx) {
							int newX = x + dx;
							int newY = y + dy;
							int newZ = slice + dz;
							if (newX >= 0 && newX < width && newY >= 0 && newY < height && newZ >= 0 && newZ < slices) {
								unsigned char* currentSlice = data[newZ];
								for (int channel = 0; channel < 3; ++channel) {
									newPixel[channel] += kernel[dx + offset][dy + offset][dz + offset] * currentSlice[(newY * width + newX) * 3 + channel];
								}
							}
						}
					}
				}
				for (int channel = 0; channel < 3; ++channel) {
					newSlice[(y * width + x) * 3 + channel] = static_cast<unsigned char>(std::min(std::max(int(newPixel[channel]), 0), 255));
				}
			}
		}
		newData.push_back(newSlice);
		++slice;
		count++;
		//clearScreen();
	}
	std::cout << "Count: " << count << std::endl;
	// Replace old data
	//for (auto& slice : data) {
	//	stbi_image_free(slice);
	//}
	//data = newData;
	volume.setData(newData);
}

void GaussianFilter3D::info(){
    std::cout << "Gaussian Filter 3D" << std::endl;
    std::cout << "Filter size: " << filterSize << std::endl;
    std::cout << "Sigma: " << sigma << std::endl;
}

void GaussianFilter3D::setFilter(int filterSize, double sigma){
    this->filterSize = filterSize;
    this->sigma = sigma;
}