/**
 * @file Helpers.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of utility functions for various simple operations.
 *
 * This file declares a series of utility functions in the helpers namespace. These functions
 * include general-purpose operations such as printing messages, formatting prices, clearing the screen,
 * pausing the program, and more advanced operations like ensuring directory existence.
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

#ifndef HELPERS_H 
#define HELPERS_H
#include <iostream>
#include <filesystem>
#include <numeric>
#include <set>
#include <vector>
namespace fs = std::filesystem;

 /**
 * @enum Align
 * @brief Alignment options for text.
 */

enum class Align { Left, Center, Right };

/**
 * @namespace helpers
 * @brief Namespace containing utility functions.
 */

namespace helpers {

     /**
     * @brief Print a message to standard output.
     * @param message The message to print.
     */
	void print(const std::string& message);	
	
	/**
     * @brief Center-aligns a string within a given width.
     * @param str String to center.
     * @param width Width of the field within which to center the string.
     * @return A new string that is centered.
     */
	std::string centered(const std::string&, int);	
	
	/**
     * @brief Formats a double value as a price string.
     * @param price The price to format.
     * @return A formatted price string.
     */
	std::string formatPrice(double);	

	/**
     * @brief Clears the console screen.
     */
	void clearScreen();		
	
	/**
     * @brief Pauses program execution and waits for the user to press Enter.
     */
	void pause();

	/**
     * @brief Prints a horizontal line of a specified length.
     * @param len The length of the line to print. Defaults to 80 characters.
     */										
	void printLine(int len = 80);
	
	/**
     * @brief Prints a title with specified alignment and optional border lines.
     * @param title The title to print.
     * @param totalLength The total length of the line.
     * @param align The alignment of the title.
     * @param n Specifies if borders should be printed. A value of 1 prints top and bottom borders.
     */
	void printTitle(const std::string&, int, Align, int);
	
	/**
     * @brief Gets integer input from the user between 0 and 100.
     * @param number Reference to an integer to store the input.
     * @return The entered integer.
     */
	int getInput(int&);	
	
	/**
     * @brief Prints signature details for the program or script.
     */
	void printSigniture();	
	
	/**
     * @brief Ensures that a specified directory exists, creating it if necessary.
     * @param path The filesystem path to the directory.
     * @param verbose Specifies whether to print messages about the operation.
     */
    void ensure_directory_exists(const fs::path& path, bool verbose = false); 
	
	/**
     * @brief Generates and returns a vector filled with a sequence of integers.
     * @param size The size of the vector to generate.
     * @return A vector of integers.
     */
	std::vector<int> get_vector(int size);

	/**
     * @brief Generates and returns a set filled with a sequence of integers.
     * @param size The size of the set to generate.
     * @return A set of integers.
     */
	std::set<int> get_set(int size);
}

#endif // HELPERS_H
