/**
 * @file UI2D.cpp
 * @author Zeqi Li (acse-zl123)
 * @brief Implements the UI2D class for interacting with users.
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
#include "../include/UI2D.h"
#include "../include/Helpers.h"
using namespace helpers;
int UI2D::run() {
    mainMenu2D();
    return 0;
}

int UI2D::mainMenu2D() {
    clearScreen();
    std::cout << "Welcome to 2D filters!" << std::endl;
    std::cout << "Please enter the path of the input image: ";
    std::string imgFilePath;
    getline(std::cin, imgFilePath);
    Image I(imgFilePath);

    bool continueProcessing = true;
    while (continueProcessing)
    {
        std::cout << "Select the type of filter:" << std::endl;
        std::cout << "1. Grayscale" << std::endl;
        std::cout << "2. Brightness" << std::endl;
        std::cout << "3. Histogram Equalisation" << std::endl;
        std::cout << "4. Thresholding" << std::endl;
        std::cout << "5. Salt and Pepper Noise" << std::endl;
        std::cout << "6. Blur" << std::endl;
        std::cout << "7. Edge Detection" << std::endl;
        std::cout << "8. Finish Processing" << std::endl;
        std::cout << "0. Exit" << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 0) {
            return 0;
		}
		else
        if (choice == 1) {
            if (I.getChannels() == 1)
            {
                std::cout << "The image is already grayscale." << std::endl;
                continue;
            }
            else
            {
                Grayscale grayscale_filter;
                grayscale_filter.apply(I);
            }
        }
        else if (choice == 2) {
            int brightness;
            std::cout << "Enter the brightness value: ";
            std::cin >> brightness;
            std::cin.ignore();
            Brightness brightness_filter(brightness);
            brightness_filter.apply(I);
        }
        else if (choice == 3) {
            if (I.getChannels() == 1) {
                HistogramEqualisation histogram_equalisation_filter(true);
                histogram_equalisation_filter.apply(I);
            }
            else {
                std::cout << "The image has multiple channels. Select the method for histogram equalisation:" << std::endl;
                std::cout << "1. HSV (Hue, Saturation, Value)" << std::endl;
                std::cout << "2. HSL (Hue, Saturation, Lightness)" << std::endl;
                int methodChoice;
                std::cin >> methodChoice;
                std::cin.ignore();
                if (methodChoice == 1) {
                        HistogramEqualisation histogram_equalisation_hsv_filter(true);
                        histogram_equalisation_hsv_filter.apply(I);
                }
                else if (methodChoice == 2) {
                        HistogramEqualisation histogram_equalisation_hsl_filter(true);
                        histogram_equalisation_hsl_filter.apply(I);
                }
                else {
                        std::cout << "Invalid method choice. Please select again." << std::endl;
                        continue;
                }
            }
        }
        else if (choice == 4) {
            double threshold;
            std::cout << "Enter the threshold value (0 to 255): ";
            std::cin >> threshold;
            std::cin.ignore();
            if (I.getChannels() == 1) {
                Thresholding thresholding_filter(threshold, true);
                thresholding_filter.apply(I);
            }
            else {
                std::cout << "The image has multiple channels. Select the method for thresholding:" << std::endl;
                std::cout << "1. HSV (Hue, Saturation, Value)" << std::endl;
                std::cout << "2. HSL (Hue, Saturation, Lightness)" << std::endl;
                int methodChoice;
                std::cin >> methodChoice;
                std::cin.ignore();
                if (methodChoice == 1) {
                        Thresholding thresholding_hsv_filter(threshold, true);
                        thresholding_hsv_filter.apply(I);
                }
                else if (methodChoice == 2) {
                        Thresholding thresholding_hsl_filter(threshold, false);
                        thresholding_hsl_filter.apply(I);
                }
                else {
                        std::cout << "Invalid method choice. Please select again." << std::endl;
                        continue;
                }
            }
        }
        else if (choice == 5) {
            double noisePercentage;
            std::cout << "Enter the percentage of noise (0 to 100): ";
            std::cin >> noisePercentage;
            std::cin.ignore();
            SaltAndPepperNoise salt_and_pepper_noise_filter(noisePercentage);
            salt_and_pepper_noise_filter.apply(I);
        }
        else if (choice == 6) {
            std::cout << "Select the type of blur:" << std::endl;
            std::cout << "1. Median blur" << std::endl;
            std::cout << "2. Box blur" << std::endl;
            std::cout << "3. Gaussian blur" << std::endl;
            int blurChoice;
            std::cin >> blurChoice;
            std::cin.ignore();
            int kernelSize;
            std::cout << "Enter the kernel size (e.g., 3 for 3x3 kernel): ";
            std::cin >> kernelSize;
            std::cin.ignore();
            double sigma;
            std::cout << "Enter the sigma value: ";
            std::cin >> sigma;
            std::cin.ignore();
            std::string type;
            if (blurChoice == 1) {
                type = "median";
                Kernel K(kernelSize, type, sigma);
                Blur B(K);
                B.apply(I);
            }
            else if (blurChoice == 2) {
                type = "box";
                Kernel K(kernelSize, type, sigma);
                Blur B(K);
                B.apply(I);
            }
            else if (blurChoice == 3) {
                type = "gaussian";
                Kernel K(kernelSize, type, sigma);
                Blur B(K);
                B.apply(I);
            }
            else {
                std::cout << "Invalid blur choice. Please select again." << std::endl;
                continue;
            }
        }
        else if (choice == 7) {
            if (I.getChannels() != 1)
            {
                Grayscale grayscale_filter;
                grayscale_filter.apply(I);
            }

            char applyBlurChoice;
            std::cout << "Do you want to apply blur before edge detection? (y/n) (Applying blur can enhance the edge detection effect): ";
            std::cin >> applyBlurChoice;
            std::cin.ignore();

            if (applyBlurChoice == 'y' || applyBlurChoice == 'Y') {
                std::cout << "Select the type of blur:" << std::endl;
                std::cout << "1. Median blur" << std::endl;
                std::cout << "2. Box blur" << std::endl;
                std::cout << "3. Gaussian blur" << std::endl;
                int blurChoice;
                std::cin >> blurChoice;
                std::cin.ignore();
                int kernelSize;
                std::cout << "Enter the kernel size (e.g., 3 for 3x3 kernel): ";
                std::cin >> kernelSize;
                std::cin.ignore();
                double sigma;
                std::cout << "Enter the sigma value: ";
                std::cin >> sigma;
                std::cin.ignore();
                std::string type;
                if (blurChoice == 1) {
                        type = "median";
                        Kernel K(kernelSize, type, sigma);
                        Blur B(K);
                        B.apply(I);
                }
                else if (blurChoice == 2) {
                        type = "box";
                        Kernel K(kernelSize, type, sigma);
                        Blur B(K);
                        B.apply(I);
                }
                else if (blurChoice == 3) {
                        type = "gaussian";
                        Kernel K(kernelSize, type, sigma);
                        Blur B(K);
                        B.apply(I);
                }
                else {
                        std::cout << "Invalid blur choice. Please select again." << std::endl;
                        continue;
                }
            }

            std::cout << "Select the type of edge detection:" << std::endl;
            std::cout << "1. Sobel (3x3 operators)" << std::endl;
            std::cout << "2. Prewitt (3x3 operators)" << std::endl;
            std::cout << "3. Scharr (3x3 operators)" << std::endl;
            std::cout << "4. Roberts' Cross (2x2 operators)" << std::endl;
            int edgeChoice;
            std::cin >> edgeChoice;
            std::cin.ignore();
            if (edgeChoice == 1)
            {
                EdgeDetection edge_detector_sobel_filter(EdgeDetectionAlgorithm::Sobel);
                edge_detector_sobel_filter.apply(I);
            }
            else if (edgeChoice == 2)
            {
                EdgeDetection edge_detector_prewitt_filter(EdgeDetectionAlgorithm::Prewitt);
                edge_detector_prewitt_filter.apply(I);
            }
            else if (edgeChoice == 3)
            {
                EdgeDetection edge_detector_scharr_filter(EdgeDetectionAlgorithm::Scharr);
                edge_detector_scharr_filter.apply(I);
            }
            else if (edgeChoice == 4)
            {
                EdgeDetection edge_detector_roberts_filter(EdgeDetectionAlgorithm::Roberts);
                edge_detector_roberts_filter.apply(I);
            }
            else
            {
                std::cout << "Invalid edge detection choice. Please select again." << std::endl;
                continue;
            }
        }
        else if (choice == 8) {
            std::cout << "Exit successfully!" << std::endl;
            continueProcessing = false;
            break;
        }
        else {
            std::cout << "Invalid option, please select again." << std::endl;
            continue;
        }

        std::string outputPath;
        std::cout << "The image has been successfully processed. Please enter the path for the output image: ";
        getline(std::cin, outputPath);
        I.outputImage(outputPath);
        std::cout << "Image saved successfully!" << std::endl;

        if (continueProcessing) {
            char continueChoice;
            std::cout << "Do you want to add another filter? (y/n): ";
            std::cin >> continueChoice;
            std::cin.ignore();
            if (continueChoice != 'y' && continueChoice != 'Y') {
                std::cout << "Exit successfully!" << std::endl;
                continueProcessing = false;
            }
        }
    }
    return 0;
}