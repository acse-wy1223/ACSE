/**
 * @file Helpers.cpp
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of utility functions for various simple operations.
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

#include "../include/Helpers.h"
#include <iostream>
#include <iomanip> 
#include <sstream> 
#include <cstdlib> 
#include <limits>

namespace helpers {
	void print(const std::string& message) {
		std::cout << message << std::endl;
	}

	std::string centered(const std::string& str, int width) {
		int len = str.length();
		if (width < len) return str;
		int pad_left = (width - len) / 2;
		int pad_right = width - len - pad_left;
		return std::string(pad_left, ' ') + str + std::string(pad_right, ' ');
	}

	std::string formatPrice(double price) {
		std::ostringstream out;
		out << std::fixed << std::setprecision(1) << price;
		return out.str();
	}

	void clearScreen() {
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
	}

	void pause() {
		std::cout << "Press Enter to continue...";
		// clear the input buffer
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		// wait for the user to press enter
		std::cin.get();
	}

	void printLine(int len) {
		std::cout << std::string(len, '-') << std::endl;
	}

	void printTitle(const std::string& title, int totalLength, Align align, int n) {
		int titleLength = title.size();
		if (titleLength + 2 > totalLength) { // +2 for the two spaces
			std::cout << title << std::endl; // if the title is too long, just print it as is
			return;
		}

		int padding = totalLength - titleLength - 2; // calculate the padding needed
		int leftPadding{}, rightPadding{};

		switch (align) {
		case Align::Left:
			leftPadding = 1;
			rightPadding = padding;
			break;
		case Align::Center:
			leftPadding = padding / 2 + 1;
			rightPadding = padding - padding / 2 + 1;
			break;
		case Align::Right:
			leftPadding = padding;
			rightPadding = 1;
			break;
		}
		if (n == 1) {
			std::cout << std::string(totalLength + 2, '-') << std::endl;
		}
		std::cout << std::string(leftPadding, '-') << ' ' << title << ' ' << std::string(rightPadding, '-') << std::endl;
		if (n == 1) {
			std::cout << std::string(totalLength + 2, '-') << std::endl;
		}
	}

	int getInput(int& number) {
		while (true) {
			//std::cout << "Please enter a number between 0 and 100: ";
			std::cin >> number;

			if (!std::cin.fail() && number >= 0 && number <= 100) {
				return number;
			}
			else {
				std::cout << "Invalid input. ";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}

	void printSigniture() {
		// Author information
		std::string name = "Wenbo Yu";
		std::string email = "wy1223@ic.ac.uk";
		std::string description = "Advanced Programming 2023/2024 Coursework–Part B";

		// Print author information
		std::cout << "Author Name: " << name << std::endl;
		std::cout << "Email: " << email << std::endl;
		std::cout << "Description: " << description << std::endl;
		std::cout << std::string(63, '-') << std::endl;
	}

    void ensure_directory_exists(const fs::path& path, bool verbose) {
        // Check if the directory exists
        if (!fs::exists(path)) {
            // Try to create the directory
            std::error_code ec; // Receive error code
            if (fs::create_directories(path, ec)) { // Succeeded
                if (verbose)
                    std::cout << "Directory created: " << path << std::endl;
            } else { // Print error message
                if (verbose)
                    std::cerr << "Failed to create directory: " << path
                            << " (" << ec.message() << ")" << std::endl;
            }
        } else {
            if (verbose)
                std::cout << "Directory already exists: " << path << std::endl;
        }
    }

	std::vector<int> get_vector(int size)
	{
		std::vector<int> A(size);
		std::iota(A.begin(), A.end(), 1000);
		//     std::generate(A.begin(), A.end(), []() {return rand(); } );

		return A;
	}

	std::set<int> get_set(int size)
	{
		std::vector<int> A(size);
		std::iota(A.begin(), A.end(), 1000);
		//     std::generate(A.begin(), A.end(), []() {return rand(); } );

		return std::set<int>(A.begin(), A.end());
	}
}
