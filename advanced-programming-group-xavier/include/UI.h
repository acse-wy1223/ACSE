/**
 * @file UI.h
 * @author Wenbo Yu (acse-wy1223)
 * @brief Declaration of the UI class that handles the main user interface for the application.
 *
 * This file contains the declaration of the UI class which orchestrates the user interface
 * and interaction for a console-based application that allows switching between 2D and 3D UI contexts.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI_H

#include "UI2D.h"
#include "UI3D.h"
#include "Helpers.h"
using namespace helpers;

/**
 * @class UI
 * @brief Main User Interface class for handling 2D and 3D UI operations.
 *
 * This class contains methods to run the main menu of the application and to delegate control
 * to either 2D or 3D user interface classes based on user input. It provides an entry point
 * for user interaction with the system.
 */
class UI
{
public:

   /**
     * @brief Default constructor for the UI class.
     */
	UI() {};

	/**
     * @brief Default destructor for the UI class.
     */
	~UI() {};

	/**
     * @brief Runs the UI interaction loop.
     */
	void run() {
		mainMenu();
	}

   /**
     * @brief Displays the main menu and handles user interaction.
     * @return Integer indicating the status upon exit, with 0 for normal termination.
     */
	int mainMenu() {
		while (true)
		{
			clearScreen();
			print("3D or 2D?");
			print("1. 2D");
			print("2. 3D");
			print("0. Exit");
			int option;
			std::cin >> option;
			// Check if the input is valid
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
				print("Please enter a valid number.");
				pause();
				continue;
			}
			else { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
			if (option == 1) {
				print("Your choice is 2D");
				UI2D ui2d;
				ui2d.run();
			}
			else if (option == 2) {
				print("Your choice is 3D");
				UI3D ui3d;
				ui3d.run();
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
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UI_H
