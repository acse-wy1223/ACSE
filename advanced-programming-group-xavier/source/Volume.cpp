/**
 * @file Volume.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the Volume class for managing 3D volume data.
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

#include <cstring>
#include <random>
#include <chrono>
#include "../include/Volume.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/Helpers.h"
#include "../include/tqdm.hpp"

using namespace helpers;

Volume::~Volume() {

	for (auto& slice : data) {
		stbi_image_free(slice);
	}

	for (auto& slice : originalData) {
		stbi_image_free(slice);
	}
}

void Volume::setData(const std::vector<unsigned char*>& newData) {
	// Clean up existing data
	for (auto& slice : data) {
		stbi_image_free(slice);
	}
	data.clear();

	// Use cloneData to deep copy newData into data
	cloneData(newData, data);

	// Optionally, you can also update slices based on newData size
	slices = newData.size();
}

bool Volume::loadVolume(const std::string& directoryPath) {
	for (const auto& entry : fs::directory_iterator(directoryPath)) 
    //for (const auto& entry : T)
    {
		int w, h, channels;
        //clearScreen();
		std::cout << "Loading Volume from: " << entry.path().string().c_str() << std::endl;
		unsigned char* img = stbi_load(entry.path().string().c_str(), &w, &h, &channels, 3);
		if (img == nullptr) {
			std::cerr << "Failed to load image: " << entry.path() << std::endl;
			return false;
		}
		if (width == 0 && height == 0) {
            this->width = w;
            this->height = h;
            this->channels = channels;
		}
		else if (w != width || h != height) {
			std::cerr << "Image dimensions do not match: " << entry.path() << std::endl;
			return false;
		}
		data.push_back(img);
	}
	slices = data.size();
	cloneData(data, originalData); // Clone originalData to data
	return true;
}

void Volume::saveVolume(const std::string& directoryPath) {
	int index = 0;
	for (const auto& slice : data) {
		std::cout << "Saving " << index << std::endl;
		std::string filename = directoryPath + "/slice_" + std::to_string(index++) + ".png";
		std::cout << "Saving " << filename << std::endl;
		stbi_write_png(filename.c_str(), width, height, 3, slice, width * 3);
	}
}

void Volume::saveSlice(const std::string& outputPath, const unsigned char* sliceData, int sliceWidth, int sliceHeight) const {
	if (!stbi_write_png(outputPath.c_str(), sliceWidth, sliceHeight, 3, sliceData, sliceWidth * 3)) {
		std::cerr << "Failed to save slice image to: " << outputPath << std::endl;
	}
	//int success = stbi_write_png(imagePathStr.c_str(), w, h, c, data, 0);
}

void Volume::reloadVolume() {
	for (auto& slice : data) {
		stbi_image_free(slice);
	}
	data.clear();
	cloneData(originalData, data); // Clone originalData to data
}

void Volume::cloneData(const std::vector<unsigned char*>& source, std::vector<unsigned char*>& destination) {
	destination.clear();
	for (const auto& slice : source) {
		unsigned char* cloneSlice = new unsigned char[width * height * 3];
		std::memcpy(cloneSlice, slice, width * height * 3);
		destination.push_back(cloneSlice);
	}
}

void Volume::generateSamples(const std::string& directory, int count, int width, int height, int seed) {
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);

    for (int i = 0; i < count; ++i) {
        std::vector<unsigned char> image(width * height * 3); // RGB
        for (int j = 0; j < width * height * 3; j += 3) {
            image[j] = dist(rng); // R
            image[j + 1] = dist(rng); // G
            image[j + 2] = dist(rng); // B
        }

        std::string filename = directory + "/sample_" + std::to_string(i+1) + ".png";
        stbi_write_png(filename.c_str(), width, height, 3, image.data(), 0);
    }
}

void Volume::readAndPrintSamples(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        int width, height, channels;
        unsigned char* img = stbi_load(entry.path().string().c_str(), &width, &height, &channels, 0);

        if (img == nullptr) {
            std::cout << "Failed to load image: " << entry.path() << std::endl;
            continue;
        }

        std::cout << "Image: " << entry.path().filename() << " - Size: " << width << "x" << height << std::endl;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char* pixel = img + (y * width + x) * channels;
                std::cout << "(" << static_cast<int>(pixel[0]) << ", " << static_cast<int>(pixel[1]) << ", " << static_cast<int>(pixel[2]) << ") ";
            }
            std::cout << std::endl;
        }

        stbi_image_free(img);
    }
}

void Volume::printVolumeData() const {
    for (size_t sliceIndex = 0; sliceIndex < data.size(); ++sliceIndex) {
        const unsigned char* slice = data[sliceIndex];
        std::cout << "Slice " << sliceIndex << ":" << std::endl;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int pixelIndex = (y * width + x) * 3;
                std::cout << "("
                          << static_cast<int>(slice[pixelIndex]) << ", " // R
                          << static_cast<int>(slice[pixelIndex + 1]) << ", " // G
                          << static_cast<int>(slice[pixelIndex + 2]) // B
                          << ") ";
            }
            std::cout << std::endl; 
        }
        std::cout << std::endl; 
    }
}

std::vector<unsigned char> Volume::getVolumePixelData() const {
    size_t totalBytes = slices * width * height * 3;
    std::vector<unsigned char> volumeData(totalBytes);

    for (size_t sliceIndex = 0; sliceIndex < data.size(); ++sliceIndex) {
        const unsigned char* sliceData = data[sliceIndex];
        size_t startIdx = sliceIndex * width * height * 3;
        std::copy_n(sliceData, width * height * 3, volumeData.begin() + startIdx);
    }

    return volumeData;
}

std::vector<unsigned char> Volume::getImagePixelData(const std::string& path) const {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return {};
    }

    std::vector<unsigned char> pixelData(data, data + width * height * channels);

    stbi_image_free(data);

    return pixelData;
}


