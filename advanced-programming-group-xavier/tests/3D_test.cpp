#include <chrono>
#include <math.h>
#include "3D_test.h"
#include "../include/GaussianFilter3D.h"
#include "../include/MedianFilter3D.h"
#include "../include/Volume.h"
#include "../include/Projection.h"
#include "../include/Slice.h"
#include "../include/GaussianFilter3D.h"
#include "../include/MedianFilter3D.h"

Test3D::Test3D() {
    this->unit_inputDir = "../tests/data/TestSamples3D";
	this->unit_outputDir = "../tests/data/TestSamples3D_Output";
	this->result_inputDir = "../Scans/";
	this->result_outputDir = "../Output/8-3D/";
}

Test3D::Test3D(const std::string& unit_inputDir, const std::string& unit_outputDir, const std::string& result_inputDir, const std::string& result_outputDir) {
	this->unit_inputDir = unit_inputDir;
	this->unit_outputDir = unit_outputDir;
	this->result_inputDir = result_inputDir;
	this->result_outputDir = result_outputDir;
}

Test3D::~Test3D() {
}

bool Test3D::comparePixelVectors(const std::vector<unsigned char>& processedPixels, const std::vector<unsigned char>& truePixels, float tolerance) {
    if (processedPixels.size() != truePixels.size()) {
        std::cout << "Error: Pixel vectors are of different sizes." << std::endl;
        return false;
    }

    for (size_t i = 0; i < processedPixels.size(); i++) {
        // 将unsigned char转换为int进行差值计算，避免负数
        int diff = static_cast<int>(processedPixels[i]) - static_cast<int>(truePixels[i]);
        if (std::abs(diff) > tolerance * 255) {
            std::cout << "Pixel difference exceeded tolerance at index " << i
                      << ". Processed pixel: " << static_cast<int>(processedPixels[i])
                      << ", True pixel: " << static_cast<int>(truePixels[i]) << std::endl;
            return false;
        }
    }

    return true;
}

bool Test3D::testVolume(const std::string& inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}

    Volume volume;
    volume.loadVolume(input);
    if (verbose) {
        std::cout << "Volume Test:" << std::endl;
        std::cout << "Volume size: " << volume.getWidth() << " x " << volume.getHeight() << " x " << volume.getChannels() << " x " << volume.getSlices() << std::endl;
    }
    if (volume.getWidth() == 3 && volume.getHeight() == 3 && volume.getChannels() == 3 && volume.getSlices() == 4) {
        return true;
    }else {
		throw std::runtime_error("Volume loading incorrect.");
        return false;
    }
}

bool Test3D::testGaussianFilter3D(const std::string& inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
		truePixels = {
			61, 50, 49, 82, 65, 65, 58, 46, 45, 81, 69, 66, 115, 90, 82, 83, 61, 52, 57, 51, 43, 83, 66, 51, 61, 42, 29,       // sample 1
			75, 65, 61, 104, 89, 84, 71, 64, 61, 101, 97, 85, 144, 127, 116, 104, 85, 84, 74, 77, 62, 108, 96, 81, 79, 61, 57, // sample 2
			62, 54, 53, 90, 90, 85, 63, 74, 71, 88, 88, 80, 127, 131, 122, 90, 97, 100, 70, 75, 66, 99, 102, 93, 70, 70, 72,   // sample 3
			40, 33, 36, 60, 67, 66, 45, 62, 61, 60, 55, 57, 87, 97, 92, 62, 80, 78, 52, 47, 49, 70, 76, 69, 46, 57, 52, };
#else
	return true;
#endif
    // sample 4
    Volume volume;
    volume.loadVolume(input);
    GaussianFilter3D filter(3, 1.0);
    filter.apply(volume);
    std::vector<unsigned char> processedPixels = volume.getVolumePixelData();
    if (verbose) {
        std::cout << "GaussianFilter3D Test:" << std::endl;
    }
    // print the processed pixels and the true pixels
    if (verbose) {
        std::cout << "Processed pixels: ";
        for (size_t i = 0; i < processedPixels.size(); ++i) {
            std::cout << static_cast<int>(processedPixels[i]) << ", ";
        }
        std::cout << std::endl;
        std::cout << "True pixels: ";
        for (size_t i = 0; i < truePixels.size(); ++i) {
            std::cout << static_cast<int>(truePixels[i]) << ", ";
        }
        std::cout << std::endl;
    }

    if (comparePixelVectors(processedPixels, truePixels)) {
        return true;
    }else {
		throw std::runtime_error("Gaussian3D Filter incorrect.");
        return false;
    }
}

bool Test3D::testMedianFilter3D(const std::string& inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = {
		177, 161, 152, 148, 125, 152, 148, 125, 175, 177, 156, 91, 160, 123, 91, 160, 123, 82, 160, 156, 91, 160, 123, 91, 167, 123, 82,          // sample 1
		136, 110, 136, 136, 123, 136, 141, 125, 175, 160, 156, 109, 136, 118, 109, 148, 123, 111, 136, 156, 109, 136, 118, 109, 160, 118, 109,    // sample 2
		126, 127, 106, 126, 125, 111, 126, 161, 111, 145, 156, 109, 132, 125, 111, 132, 139, 111, 136, 162, 123, 132, 125, 123, 132, 125, 131,    // sample 3
		126, 110, 106, 126, 111, 111, 126, 161, 156, 126, 127, 123, 126, 127, 123, 128, 161, 154, 126, 192, 123, 126, 118, 131, 128, 139, 154, };  // sample 4
#else
	return true;
#endif

    Volume volume;
    volume.loadVolume(input);
    MedianFilter3D filter(3);
    filter.apply(volume);
    std::vector<unsigned char> processedPixels = volume.getVolumePixelData();
    if (verbose) {
        std::cout << "MedianFilter3D Test:" << std::endl;
    }
    // print the processed pixels and the true pixels
    if (verbose) {
        std::cout << "Processed pixels: ";
        for (size_t i = 0; i < processedPixels.size(); ++i) {
            std::cout << static_cast<int>(processedPixels[i]) << ", ";
        }
        std::cout << std::endl;
        std::cout << "True pixels: ";
        for (size_t i = 0; i < truePixels.size(); ++i) {
            std::cout << static_cast<int>(truePixels[i]) << ", ";
        }
        std::cout << std::endl;
    }

    if (comparePixelVectors(processedPixels, truePixels)) {
        return true;
    }else {
		throw std::runtime_error("Median3D Filter incorrect.");
        return false;
    }
}

bool Test3D::testProjectionAIP(const std::string &inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = { 139, 93, 94, 157, 126, 131, 112, 161, 137,   // row 1
					83, 106, 125, 153, 139, 104, 129, 109, 150,    // row 2
					147, 120, 132, 183, 184, 112, 119, 102, 94 // row 3
}; 
#else
	return true;
#endif

    Volume volume;
    volume.loadVolume(input);
    Projection projection;
    projection.applyAIP(volume, output + "/AIP.png");
    std::vector<unsigned char> projectionPixelData = volume.getImagePixelData(output + "/AIP.png");
    if (verbose) {
        std::cout << "AIP Test:" << std::endl;
    }
    // print the processed pixels and the true pixels
    if (verbose) {
        std::cout << "Processed pixels: ";
        for (size_t i = 0; i < projectionPixelData.size(); ++i) {
            std::cout << static_cast<int>(projectionPixelData[i]) << ", ";
        }
        std::cout << std::endl;
        std::cout << "True pixels: ";
        for (size_t i = 0; i < truePixels.size(); ++i) {
            std::cout << static_cast<int>(truePixels[i]) << ", ";
        }
        std::cout << std::endl;
    }
    // compare the processed pixels with the true pixels
    if (comparePixelVectors(projectionPixelData, truePixels)) {
        return true;
    }else {
		throw std::runtime_error("AIP incorrect.");
        return false;
    }
}

bool Test3D::testProjectionMIP(const std::string &inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = { 178, 182, 152, 252, 161, 196, 174, 221, 234, // row 1
										136, 182, 199, 251, 252, 175, 217, 125, 227,   // row 2
										192, 192, 228, 211, 241, 154, 167, 139, 159 }; // row 3
#else
	return true;
#endif

	Volume volume;
	volume.loadVolume(input);
	Projection projection;
	projection.applyMIP(volume, output + "/MIP.png");
	std::vector<unsigned char> projectionPixelData = volume.getImagePixelData(output + "/MIP.png");
	if (verbose) {
		std::cout << "MIP Test:" << std::endl;
	}
	// print the processed pixels and the true pixels
	if (verbose) {
		std::cout << "Processed pixels: ";
		for (size_t i = 0; i < projectionPixelData.size(); ++i) {
			std::cout << static_cast<int>(projectionPixelData[i]) << ", ";
		}
		std::cout << std::endl;
		std::cout << "True pixels: ";
		for (size_t i = 0; i < truePixels.size(); ++i) {
			std::cout << static_cast<int>(truePixels[i]) << ", ";
		}
		std::cout << std::endl;
	}
	// compare the processed pixels with the true pixels
	if (comparePixelVectors(projectionPixelData, truePixels)) {
		return true;
	}else {
		throw std::runtime_error("MIP incorrect.");
		return false;
	}
}

bool Test3D::testProjectionMinIP(const std::string& inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = { 58, 29, 73, 81, 58, 46, 23, 44, 21,	  // row 1
					7, 27, 44, 109, 39, 59, 19, 79, 35,		  // row 2
					100, 30, 87, 160, 106, 82, 72, 62, 15, }; // row 3
#else
	return true;
#endif

	Volume volume;
	volume.loadVolume(input);
	Projection projection;
	projection.applyMinIP(volume, output + "/MinIP.png");
	std::vector<unsigned char> projectionPixelData = volume.getImagePixelData(output + "/MinIP.png");
	if (verbose) {
		std::cout << "MinIP Test:" << std::endl;
	}
	// print the processed pixels and the true pixels
	if (verbose) {
		std::cout << "Processed pixels: ";
		for (size_t i = 0; i < projectionPixelData.size(); ++i) {
			std::cout << static_cast<int>(projectionPixelData[i]) << ", ";
		}
		std::cout << std::endl;
		std::cout << "True pixels: ";
		for (size_t i = 0; i < truePixels.size(); ++i) {
			std::cout << static_cast<int>(truePixels[i]) << ", ";
		}
		std::cout << std::endl;
	}
	// compare the processed pixels with the true pixels
	if (comparePixelVectors(projectionPixelData, truePixels)) {
		return true;
	}
	else {
		throw std::runtime_error("MinIP incorrect.");
		return false;
	}
}

bool Test3D::testSliceYZ(const std::string &inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = { 109, 58, 176, 188, 161, 46, 252, 161, 196,	 // row 1
										  81, 127, 106, 251, 105, 175, 109, 162, 76,	 // row 2
										  126, 39, 109, 126, 252, 59, 160, 186, 82,      // row 3
										  184, 106, 82, 177, 241, 131, 211, 204, 154, }; // row 4
#else
	return true;
#endif

    Volume volume;
    volume.loadVolume(input);
    Slice slice;
    slice.dosliceYZ(volume, 2, output + "/YZ.png");
    std::vector<unsigned char> slicePixelData = volume.getImagePixelData(output + "/YZ.png");
    if (verbose) {
        std::cout << "Slice Test:" << std::endl;
    }
    // print the processed pixels and the true pixels
    if (verbose) {
        std::cout << "Processed pixels: ";
        for (size_t i = 0; i < slicePixelData.size(); ++i) {
            std::cout << static_cast<int>(slicePixelData[i]) << ", ";
        }
        std::cout << std::endl;
        std::cout << "True pixels: ";
        for (size_t i = 0; i < truePixels.size(); ++i) {
            std::cout << static_cast<int>(truePixels[i]) << ", ";
        }
        std::cout << std::endl;
    }
    // compare the processed pixels with the true pixels
    if (comparePixelVectors(slicePixelData, truePixels)) {
        return true;
    }else {
		throw std::runtime_error("Slice YZ incorrect.");
        return false;
    }
}

bool Test3D::testSliceXZ(const std::string& inputDir, const std::string& outputDir, bool verbose, int mode) {
	std::string input;
	std::string output;
	if (mode == 0) {
		input = this->unit_inputDir;
		output = this->unit_outputDir;
	}
	else
	{
		input = inputDir;
		output = outputDir;
	}
	std::vector<unsigned char> truePixels;
#ifdef _WIN32
	truePixels = { 125, 108, 199, 251, 105, 175, 148, 123, 35, // row 1
										136, 182, 136, 109, 162, 76, 217, 125, 185,   // row 2
										7, 110, 44, 126, 39, 109, 19, 79, 227,		  // row 3
										64, 27, 123, 126, 252, 59, 132, 111, 156, };  // row 4
#else
	return true;
#endif

	Volume volume;
	volume.loadVolume(input);
	Slice slice;
	slice.dosliceXZ(volume, 2, output + "/XZ.png");
	std::vector<unsigned char> slicePixelData = volume.getImagePixelData(output + "/XZ.png");
	if (verbose) {
		std::cout << "Slice Test:" << std::endl;
	}
	// print the processed pixels and the true pixels
	if (verbose) {
		std::cout << "Processed pixels: ";
		for (size_t i = 0; i < slicePixelData.size(); ++i) {
			std::cout << static_cast<int>(slicePixelData[i]) << ", ";
		}
		std::cout << std::endl;
		std::cout << "True pixels: ";
		for (size_t i = 0; i < truePixels.size(); ++i) {
			std::cout << static_cast<int>(truePixels[i]) << ", ";
		}
		std::cout << std::endl;
	}
	// compare the processed pixels with the true pixels
	if (comparePixelVectors(slicePixelData, truePixels)) {
		return true;
	}
	else {
		throw std::runtime_error("Slice XZ incorrect.");
		return false;
	}
}

void Test3D::confuciusornis_aip_gaussian3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
    std::string input;
    std::string output;

    if (mode == 0) {
        input = this->result_inputDir + "confuciusornis";
        output = this->result_outputDir + "confuciusornis/aip-gaussian_3x3x3.png";
    }
    else {
		input = inputDir;
		output = outputdir;
	}
    if (verbose) {
        std::cout << "confuciusornis_aip_gaussian3:" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(3, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyAIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
		std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
	}
}

void Test3D::confuciusornis_aip_gaussian5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/aip-gaussian_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
    if (verbose) {
        std::cout << "confuciusornis_aip_gaussian5:" << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(5, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyAIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    }
}

void Test3D::confuciusornis_aip_median3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/aip-median_3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_aip_median3:" << std::endl;
	}

    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(3);
    filter.apply(volume);
    Projection projection;
    projection.applyAIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
		std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
	}
}

void Test3D::confuciusornis_aip_median5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/aip-median_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_aip_median5:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(5);
    filter.apply(volume);
    Projection projection;
    projection.applyAIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    }
}

void Test3D::confuciusornis_aip_nofilter(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/aip-nofilter.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_aip_nofilter:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyAIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
		std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
	}
}

void Test3D::confuciusornis_mip_gaussian3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/mip-gaussian-3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_mip_gaussian3:" << std::endl;
	}

    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(3, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    if (verbose) {
		std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
	}
}

void Test3D::confuciusornis_mip_gaussian5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/mip-gaussian-5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_mip_gaussian5:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(5, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_mip_median3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/mip-median-3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_mip_median3:" << std::endl;
	}

    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(3);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_mip_median5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/mip-median-5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_mip_median5:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(5);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_mip_nofilter(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/mip-nofilter.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_mip_nofilter:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_slice_xz_y420(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/slice_xz_y420.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_slice_xz_y420:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Slice slice;
    slice.dosliceXZ(volume, 420, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_slice_yz_x400(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/slice_yz_x400.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_slice_yz_x400:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Slice slice;
    slice.dosliceYZ(volume, 400, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_thinslab_10_70_nofilter_aip(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/thinslab_10_70_nofilter_aip.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_thinslab_10_70_nofilter_aip:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyAIP(volume, output, 10, 70);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::confuciusornis_thinslab_10_70_nofilter_mip(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "confuciusornis";
		output = this->result_outputDir + "confuciusornis/thinslab_10_70_nofilter_mip.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "confuciusornis_thinslab_10_70_nofilter_mip:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyMIP(volume, output, 10, 70);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_minip_gaussian3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/minip-gaussian-3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_minip_gaussian3:" << std::endl;
	}
	
	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	GaussianFilter3D filter(3, 1.0);
	filter.apply(volume);
	Projection projection;
	projection.applyMinIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_minip_gaussian5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/minip-gaussian_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_minip_gaussian5:" << std::endl;
	}
	
	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	GaussianFilter3D filter(5, 1.0);
	filter.apply(volume);
	Projection projection;
	projection.applyMinIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_minip_median3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/minip-median_3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_minip_median3:" << std::endl;
	}
	
	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	MedianFilter3D filter(3);
	filter.apply(volume);
	Projection projection;
	projection.applyMinIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_minip_median5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/minip-median_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_minip_median5:" << std::endl;
	}
	
	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	MedianFilter3D filter(5);
	filter.apply(volume);
	Projection projection;
	projection.applyMinIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_minip_nofilter(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/minip-nofilter.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_minip_nofilter:" << std::endl;
	}
	
	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	Projection projection;
	projection.applyMinIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_mip_gaussian3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/mip-gaussian-3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_mip_gaussian3:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(3, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_mip_gaussian5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/mip-gaussian-5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_mip_gaussian5:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    GaussianFilter3D filter(5, 1.0);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_mip_median3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/mip-median-3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_mip_median3:" << std::endl;
	}

    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(3);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_mip_median5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/mip-median-5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_mip_median5:" << std::endl;
	}

    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    MedianFilter3D filter(5);
    filter.apply(volume);
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_mip_nofilter(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/mip-nofilter.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_mip_nofilter:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyMIP(volume, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_slice_xz_y138(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/slice_xz_y138.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_slice_xz_y138:" << std::endl;
	}
   
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Slice slice;
    slice.dosliceXZ(volume, 138, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_slice_yz_x275(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/slice_yz_x275.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_slice_yz_x275:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Slice slice;
    slice.dosliceYZ(volume, 275, output);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_thinslab_276_476_nofilter_aip(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/thinslab_276_476_nofilter_aip.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_thinslab_276_476_nofilter_aip:" << std::endl;
	}
  
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyAIP(volume, output, 276, 476);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_thinslab_276_476_nofilter_mip(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/thinslab_276_476_nofilter_mip.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
        std::cout << "fracture_thinslab_276_476_nofilter_mip:" << std::endl;
	}
    
    auto start = std::chrono::high_resolution_clock::now();

    Volume volume;
    if (!volume.loadVolume(input)) {
        std::cerr << "Failed to load volume" << std::endl;
    }
    Projection projection;
    projection.applyMIP(volume, output, 276, 476);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}


void Test3D::fracture_aip_gaussian3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/aip-gaussian_3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_aip_gaussian3:" << std::endl;
	}

	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	GaussianFilter3D filter(3, 1.0);
	filter.apply(volume);
	Projection projection;
	projection.applyAIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_aip_gaussian5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/aip-gaussian_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_aip_gaussian5:" << std::endl;
	}

	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	GaussianFilter3D filter(5, 1.0);
	filter.apply(volume);
	Projection projection;
	projection.applyAIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_aip_median3(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/aip-median_3x3x3.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_aip_median3:" << std::endl;
	}

	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	MedianFilter3D filter(3);
	filter.apply(volume);
	Projection projection;
	projection.applyAIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_aip_median5(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/aip-median_5x5x5.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_aip_median5:" << std::endl;
	}

	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	MedianFilter3D filter(5);
	filter.apply(volume);
	Projection projection;
	projection.applyAIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::fracture_aip_nofilter(const std::string& inputDir, const std::string& outputdir, bool verbose, int mode) {
	std::string input;
	std::string output;

	if (mode == 0) {
		input = this->result_inputDir + "fracture";
		output = this->result_outputDir + "fracture/aip-nofilter.png";
	}
	else {
		input = inputDir;
		output = outputdir;
	}
	if (verbose) {
		std::cout << "fracture_aip_nofilter:" << std::endl;
	}

	auto start = std::chrono::high_resolution_clock::now();

	Volume volume;
	if (!volume.loadVolume(input)) {
		std::cerr << "Failed to load volume" << std::endl;
	}
	Projection projection;
	projection.applyAIP(volume, output);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

void Test3D::generateOutput(bool verbose) {
	confuciusornis_aip_gaussian3("", "", verbose, 0);
	confuciusornis_aip_gaussian5("", "", verbose, 0);
	confuciusornis_aip_median3("", "", verbose, 0);
	confuciusornis_aip_median5("", "", verbose, 0);
	confuciusornis_aip_nofilter("", "", verbose, 0);
	confuciusornis_mip_gaussian3("", "", verbose, 0);
	confuciusornis_mip_gaussian5("", "", verbose, 0);
	confuciusornis_mip_median3("", "", verbose, 0);
	confuciusornis_mip_median5("", "", verbose, 0);
	confuciusornis_mip_nofilter("", "", verbose, 0);
	confuciusornis_slice_xz_y420("", "", verbose, 0);
	confuciusornis_slice_yz_x400("", "", verbose, 0);
	confuciusornis_thinslab_10_70_nofilter_aip("", "", verbose, 0);
	confuciusornis_thinslab_10_70_nofilter_mip("", "", verbose, 0);
	fracture_minip_gaussian3("", "", verbose, 0);
	fracture_minip_gaussian5("", "", verbose, 0);
	fracture_minip_median3("", "", verbose, 0);
	fracture_minip_median5("", "", verbose, 0);
	fracture_minip_nofilter("", "", verbose, 0);
	fracture_mip_gaussian3("", "", verbose, 0);
	fracture_mip_gaussian5("", "", verbose, 0);
	fracture_mip_median3("", "", verbose, 0);
	fracture_mip_median5("", "", verbose, 0);
	fracture_mip_nofilter("", "", verbose, 0);
	fracture_slice_xz_y138("", "", verbose, 0);
	fracture_slice_yz_x275("", "", verbose, 0);
	fracture_thinslab_276_476_nofilter_aip("", "", verbose, 0);
	fracture_thinslab_276_476_nofilter_mip("", "", verbose, 0);
}