#ifndef TEST3D_H
#define TEST3D_H

#include <cmath>
#include <iostream>
#include <vector>
#include "../include/Filter3D.h"
#include "../include/Projection.h"
#include "../include/Slice.h"
#include "../include/Helpers.h"

using namespace helpers;

class Test3D{
private:
public:
	std::string unit_inputDir;
	std::string unit_outputDir;
	std::string result_inputDir;
	std::string result_outputDir;
	// Constructor
	Test3D();
	Test3D(const std::string&, const std::string&, const std::string&, const std::string&);
	// Destructor
	~Test3D();
	// Setters
	void setInputDir(const std::string& inputDir);
	void setOutputDir(const std::string& outputDir);
	// Getters
	std::string getInputDir() const;
	std::string getOutputDir() const;
    // Helper Functions
    bool comparePixelVectors(const std::vector<unsigned char>& processedPixels, const std::vector<unsigned char>& truePixels, float tolerance = 0.05);
    // Unit Test
    bool testVolume(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
    bool testGaussianFilter3D(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
    bool testMedianFilter3D(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
    bool testProjectionAIP(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
	bool testProjectionMIP(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
	bool testProjectionMinIP(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
    bool testSliceYZ(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
	bool testSliceXZ(const std::string& inputDir = "", const std::string& outputDir = "", bool verbose = false, int mode = 0);
    
	//
	// Generate Output
	// Confuciusornis Test Cases
	// AIP
	void confuciusornis_aip_gaussian3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_aip_gaussian5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_aip_median3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_aip_median5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_aip_nofilter(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// MIP
	void confuciusornis_mip_gaussian3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_mip_gaussian5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_mip_median3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_mip_median5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_mip_nofilter(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Slice
	void confuciusornis_slice_xz_y420(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_slice_yz_x400(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Thinslab
	void confuciusornis_thinslab_10_70_nofilter_aip(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void confuciusornis_thinslab_10_70_nofilter_mip(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Fracture Test Cases
	// MinIP
	void fracture_minip_gaussian3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_minip_gaussian5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_minip_median3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_minip_median5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_minip_nofilter(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// MIP
	void fracture_mip_gaussian3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_mip_gaussian5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_mip_median3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_mip_median5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_mip_nofilter(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Slice
	void fracture_slice_xz_y138(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_slice_yz_x275(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Thinslab
	void fracture_thinslab_276_476_nofilter_aip(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_thinslab_276_476_nofilter_mip(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// AIP (additinal)
	void fracture_aip_gaussian3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_aip_gaussian5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_aip_median3(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_aip_median5(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	void fracture_aip_nofilter(const std::string& inputDir = "", const std::string& outputdir = "", bool verbose = false, int mode = 0);
	// Generate Output
	void generateOutput(bool verbose = false);
};


#endif // TEST3D_H