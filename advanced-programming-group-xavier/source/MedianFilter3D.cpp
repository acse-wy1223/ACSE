/**
 * @file MedianFilter3D.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Implement the MedianFilter3D class for applying a median filter to volumetric data.
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
#include "../include/MedianFilter3D.h"
#include <vector>
#include <iostream>
#include <cstring> // For memset
#include <numeric> // For std::iota
#include "../include/tqdm.hpp"
#include "../include/Helpers.h"

using namespace helpers;

MedianFilter3D::MedianFilter3D() : filterSize(3) {}
MedianFilter3D::MedianFilter3D(int filterSize) : filterSize(filterSize) {}
MedianFilter3D::~MedianFilter3D() = default;

void MedianFilter3D::info() {
    std::cout << "Median Filter 3D" << std::endl;
    std::cout << "Filter size: " << filterSize << std::endl;
}

void MedianFilter3D::setFilter(int filterSize) {
    this->filterSize = filterSize;
}

void MedianFilter3D::apply(Volume& volume) {
	int height = volume.getHeight();
	int width = volume.getWidth();
	int slices = volume.getSlices();
	auto data = volume.getData();

	int offset = filterSize / 2;
	std::vector<unsigned char*> newData(slices);

	// Histogram array for values [0, 255]
	std::vector<int> histogram(256, 0);

	auto A = get_set(slices);
	auto T = tq::tqdm(A);
	std::string times = "*";
	std::string str = "(" + std::to_string(this->filterSize) + times + std::to_string(this->filterSize) + times + std::to_string(this->filterSize) + ")";
	T.set_prefix("Applying MedianFilter3D "+ str + ":");
	int s = 0;
	//int count = 0;
	for (auto&& t : T)
	//for (int s = 0; s < slices; ++s) 
	{
		std::string str = std::to_string(s + 1) + "/" + std::to_string(slices);
		T << str;
		//count++;
		//std::cout << s + 1 << "/" << slices << std::endl;
		//std::cout << s + 1 << "/" << slices << std::endl;
		newData[s] = new unsigned char[width * height * 3];
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				for (int c = 0; c < 3; ++c) { // For each color channel
					std::fill(histogram.begin(), histogram.end(), 0); // Reset histogram
					int count = 0; // Count of pixels in the neighborhood

					for (int dz = -offset; dz <= offset; ++dz) {
						int zs = s + dz;
						if (zs >= 0 && zs < slices) {
							for (int dy = -offset; dy <= offset; ++dy) {
								int ys = y + dy;
								if (ys >= 0 && ys < height) {
									for (int dx = -offset; dx <= offset; ++dx) {
										int xs = x + dx;
										if (xs >= 0 && xs < width) {
											unsigned char value = data[zs][(ys * width + xs) * 3 + c];
											histogram[value]++;
											count++;
										}
									}
								}
							}
						}
					}

					// Find median
					int sum = 0, median = 0;
					for (int i = 0; i < 256; ++i) {
						sum += histogram[i];
						if (sum >= (count / 2) + 1) {
							median = i;
							break;
						}
					}

					newData[s][(y * width + x) * 3 + c] = static_cast<unsigned char>(median);
				}
			}
		}
		++s;
		//clearScreen();
	}
	//std::cout << "count: " << count << std::endl;
	volume.setData(newData);
}

// MedianBase implementation
//void MedianFilter3D::apply(Volume& volume) {
//    int height = volume.getHeight();
//    int width = volume.getWidth();
//    int slices = volume.getSlices();
//    std::vector<unsigned char*>  data = volume.getData();
//
//    int offset = filterSize / 2;
//    std::vector<unsigned char*> newData(slices);
//
//    for (int s = 0; s < slices; ++s) {
//        std::cout << "Filtering: " << s+1 << std::endl;
//        newData[s] = new unsigned char[width * height * 3];
//        for (int y = 0; y < height; ++y) {
//            for (int x = 0; x < width; ++x) {
//                for (int c = 0; c < 3; ++c) { // 对每个颜色通道
//                    std::vector<unsigned char> neighbourhood;
//                    for (int dz = -offset; dz <= offset; ++dz) {
//                        int zs = s + dz;
//                        if (zs >= 0 && zs < slices) {
//                            for (int dy = -offset; dy <= offset; ++dy) {
//                                int ys = y + dy;
//                                if (ys >= 0 && ys < height) {
//                                    for (int dx = -offset; dx <= offset; ++dx) {
//                                        int xs = x + dx;
//                                        if (xs >= 0 && xs < width) {
//                                            neighbourhood.push_back(data[zs][(ys * width + xs) * 3 + c]);
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    std::nth_element(neighbourhood.begin(), neighbourhood.begin() + neighbourhood.size() / 2, neighbourhood.end());
//                    newData[s][(y * width + x) * 3 + c] = neighbourhood[neighbourhood.size() / 2];
//                }
//            }
//        }
//    }
//    volume.setData(newData);
//}
