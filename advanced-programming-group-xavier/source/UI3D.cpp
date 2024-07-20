/**
 * @file UI3D.cpp
 * @author Wenbo Yu (acse-wy1223)
 * @brief Implements the UI3D class for interacting with users.
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
#include "../include/UI3D.h"

int UI3D::mainMenu() {
	while (true)
	{
		int opt = 1;
		clearScreen();
		print("Your choice is 3D");
		Volume volume;
		enterInputPath(volume, inputdir);
		print("Loading volume from: ");
		volume.loadVolume(inputdir);
		print("Volume loaded.");
		pause();
		opt = filterMenu3D(volume, inputdir, outputdir);
		if (opt == 0) {
			return 0;
		}
	}
}

int UI3D::enterInputPath(Volume& volume, std::string& path) { // pass by reference
	clearScreen();
	while (true) {
		print("Enter the path to the volume:");
		std::getline(std::cin, path);
		if (fs::exists(path)) {
			print("Your path is " + path);
			break;
		}
		else {
			print("Path does not exist. Please enter a valid path.");
			std::cin.clear(); // clear the error flag
		}
	}
	return 0;
}

std::string UI3D::enterOutputPath(std::string& path) {
	clearScreen();
	while (true) {
		print("Enter the path to save the output:");
		std::getline(std::cin, path);
		// Check if the path is valid
		fs::path outputPath(path);
		if (!outputPath.has_parent_path()) {
			print("Please enter a path with a directory.");
			continue;
		}
		// if the parent directory does not exist, create it
		fs::path parentDir = outputPath.parent_path();
		if (!fs::exists(parentDir)) {
			try {
				// Create the directory
				fs::create_directories(parentDir);
				print("Directory created: " + parentDir.string());
			}
			catch (const fs::filesystem_error& e) {
				// Display an error message if the directory cannot be created
				print(std::string("Error creating directory: ") + e.what());
				continue;
			}
		}
		print("Your path is " + path);
		return path;
	}
}

int UI3D::filterMenu3D(Volume& volume, std::string& inputdir, std::string& outputdir) {
	int filterSize = 3;
	double sigma = 1.0;
	int flag = 0;
	while (true) {
		clearScreen();
		print("Choose a filter");
		print("1. Gaussian Filter");
		print("2. Median Filter");
		print("3. Finish");
		print("0. Back");
		int option;
		std::cin >> option;
		if (option == 1) {
			flag = gaussianFilterSize(filterSize, sigma);
			GaussianFilter3D gaussianFilter3D(filterSize, sigma);
			std::cout << "Applying Gaussian filter: " << "Filter size: " << filterSize << " Sigma: " << sigma << std::endl;
			gaussianFilter3D.apply(volume);
			std::cout << "Gaussian filter applied" << std::endl;
			pause();
		}
		else if (option == 2) {
			flag = medianFilterSize(filterSize);
			MedianFilter3D medianFilter3D(filterSize);
			std::cout << "Applying Median filter: " << "Filter size: " << filterSize << std::endl;
			medianFilter3D.apply(volume);
			std::cout << "Median filter applied: " << std::endl;
			pause();
		}
		else if (option == 3) {
			operationMenu3D(volume, inputdir, outputdir);
		}
		else if (option == 0) {
			return 0;
		}
		else {
			print("Invalid option.");
		}
	}
	return 0;
}

int UI3D::gaussianFilterSize(int& filterSize, double& sigma) {
	int flag = 0;
	while (true) {
		clearScreen();
		print("Enter an odd filter kernel size greater than 0 (e.g., 3, 5, 7,...) or 0 to go back:");

		std::cout << "Your choice: ";
		int inputSize;

		while (!(std::cin >> inputSize)) {
			std::cin.clear(); // 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 
			print("Please enter a valid number.");
			std::cout << "Your choice: ";
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 

		if (inputSize == 0) {
			return 0;
		}
		else if (inputSize > 0 && inputSize % 2 == 1) { // 
			filterSize = inputSize;
			std::cout << inputSize << "x" << inputSize << "x" << inputSize << " Gaussian Filter selected." << std::endl;
			flag = gaussianSigma(filterSize, sigma);
			// pause(); 
			if (flag == 1) {
				return 1;
			}
		}
		else {
			print("Invalid option. Please enter an odd number greater than 0.");
			pause();
		}
	}
}

int UI3D::gaussianSigma(int& filterSize, double& sigma) {
	while (true) {
		clearScreen();
		print("Enter a sigma value greater than 0, or enter 0 to go back:");

		std::cout << "Your choice: ";
		double inputSigma;

		while (!(std::cin >> inputSigma)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			print("Please enter a valid number.");
			std::cout << "Your choice: ";
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (inputSigma == 0) {
			return 0;
		}
		else if (inputSigma > 0) {
			sigma = inputSigma;
			std::cout << "Sigma value " << sigma << " selected." << std::endl;
			// pause(); // 
			return 1; // 
		}
		else {
			print("Invalid option. Please enter a number greater than 0.");
			pause();
		}
	}
}

int UI3D::medianFilterSize(int& filterSize) {
	while (true) {
		clearScreen();
		print("Enter an odd filter size greater than 0 (e.g., 3, 5, 7,...) or 0 to go back:");

		std::cout << "Your choice: ";
		int inputSize;

		while (!(std::cin >> inputSize)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			print("Please enter a valid number.");
			std::cout << "Your choice: ";
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (inputSize == 0) {
			return 0;
		}
		else if (inputSize > 0 && inputSize % 2 == 1) {
			filterSize = inputSize;
			std::cout << inputSize << "x" << inputSize << "x" << inputSize << " Median Filter selected." << std::endl;
			// pause(); 
			return 1;
		}
		else {
			print("Invalid option. Please enter an odd number greater than 0.");
			// pause();
		}
	}
}

int UI3D::operationMenu3D(Volume& volume, std::string& inputdir, std::string& outputdir) {
	while (true) {
		clearScreen();
		print("Choose an operation");
		print("1. Projection");
		print("2. Slice");
		print("0. Back");
		int option;
		std::cin >> option;
		if (option == 1) {
			projectionMenu(volume, inputdir, outputdir);
		}
		else if (option == 2) {
			sliceMenu(volume, inputdir, outputdir);
		}
		else if (option == 0) {
			break;
		}
		else {
			print("Invalid option.");
		}
	}
	return 0;
}

int UI3D::projectionMenu(Volume& volume, std::string& inputdir, std::string& outputdir) {
	int start = 0;
	int end = volume.getSlices();
	while (true) {
		clearScreen();
		print("Choose a projection");
		print("1. Maximum Intensity Projection (MIP)");
		print("2. Minimum Intensity Projection (MinIP)");
		print("3. Average Intensity Projection (AIP)");
		print("0. Back");
		int option;
		std::cin >> option;
		if (option == 1) {
			print("MIP selected.");
			start = startSlice(start, end, 0, 100);
			end = endSlice(start, end, 0, 100);
			print(std::to_string(start) + " " + std::to_string(end));
			std::string outputPath = enterOutputPath(outputdir);
			Projection projection;
			std::cout << "Applying MIP projection: " << "Start slice: " << start << " End slice: " << end << std::endl;
			projection.applyMIP(volume, outputPath, start, end);
			std::cout << "MIP Finished " << outputPath << std::endl;
			std::cout << "Saving to " << outputPath << std::endl;
			pause();
		}
		else if (option == 2) {
			print("MinIP selected.");
			start = startSlice(start, end, 0, 100);
			end = endSlice(start, end, 0, 100);
			print(std::to_string(start) + " " + std::to_string(end));
			std::string outputPath = enterOutputPath(outputdir);
			Projection projection;
			std::cout << "Applying MinIP projection: " << "Start slice: " << start << " End slice: " << end << std::endl;
			projection.applyMinIP(volume, outputPath, start, end);
			std::cout << "MinIP Finished " << outputPath << std::endl;
			std::cout << "Saving to " << outputPath << std::endl;
			pause();
		}
		else if (option == 3) {
			print("AIP selected.");
			start = startSlice(start, end, 0, 100);
			end = endSlice(start, end, 0, 100);
			print(std::to_string(start) + " " + std::to_string(end));
			std::string outputPath = enterOutputPath(outputdir);
			Projection projection;
			std::cout << "Applying AIP projection: " << "Start slice: " << start << " End slice: " << end << std::endl;
			projection.applyAIP(volume, outputPath, start, end);
			std::cout << "AIP Finished " << outputPath << std::endl;
			std::cout << "Saving to " << outputPath << std::endl;
			pause();
		}
		else if (option == 0) {
			break;
		}
		else {
			print("Invalid option.");
		}
	}
	return 0;
}

int UI3D::startSlice(int start, int end, int default_start, int default_end) {
	while (true) {
		clearScreen();
		print("Enter the start slice number (" + std::to_string(default_start) + " - " + std::to_string(default_end) + "): ");
		std::cin >> start;
		if (start >= default_start && start <= default_end) {
			return start;
		}
		else {
			print("Invalid slice number.");
			pause();
		}
	}
}

int UI3D::endSlice(int start, int end, int default_start, int default_end) {
	while (true) {
		clearScreen();
		print("Enter the end slice number (" + std::to_string(default_start) + " - " + std::to_string(default_end) + "): ");
		std::cin >> end;
		if (end >= start && end <= default_end) {
			return end;
		}
		else {
			print("Invalid slice number.");
			pause();
		}
	}
}

int UI3D::sliceMenu(Volume& volume, std::string& inputdir, std::string& outputdir) {
	int index = 0;
	while (true) {
		clearScreen();
		print("Choose a slice");
		print("1. YZ Plane");
		print("2. XZ Plane");
		print("0. Back");
		int option;
		std::cin >> option;
		if (option == 1) {
			print("YZ Plane selected.");
			index = sliceIndex(100);
			print("Slice index: " + std::to_string(index));
			std::string outputPath = enterOutputPath(outputdir);
			Slice slice;
			std::cout << "Applying YZ slice: " << "Slice index: " << index << std::endl;
			slice.dosliceYZ(volume, index, outputPath);
			std::cout << "YZ Slicing Finished " << outputPath << std::endl;
			std::cout << "Saving to " << outputPath << std::endl;
			pause();
		}
		else if (option == 2) {
			print("XZ Plane selected.");
			index = sliceIndex(100);
			print("Slice index: " + std::to_string(index));
			std::string outputPath = enterOutputPath(outputdir);
			Slice slice;
			std::cout << "Applying XZ slice: " << "Slice index: " << index << std::endl;
			slice.dosliceXZ(volume, index, outputPath);
			std::cout << "XZ Slicing Finished " << outputPath << std::endl;
			std::cout << "Saving to " << outputPath << std::endl;
			pause();
		}
		else if (option == 0) {
			break;
		}
		else {
			print("Invalid option.");
		}
	}
	return 0;
}

int UI3D::sliceIndex(int default_index) {
	int index;
	while (true) {
		clearScreen();
		print("Enter the slice index: ");

		if (std::cin >> index) {
			if (index >= 0) {
				return index;
			}
			else {
				print("Invalid slice index. Index must be a non-negative integer.");
			}
		}
		else {
			print("Invalid input. Please enter an integer.");
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		pause();
	}
}