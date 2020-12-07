/*
* C++ project "Quiz" by Simon Bauer, 2020, for the Metropolia UAS course "Object oriented programming in IoT"
* This project uses C++ 17 for compiling. 
*
*/
#include "Question.h"
#include "Quiz.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define FILENAME "file.txt"

void initialize(const std::string &filename);
int main() {
	std::string filename = FILENAME;
	std::ifstream in(filename);

	std::cout << "Hello. Please enter a file name. If the file does not exist, it will be created. The file loaction is the same as the program.\nThe program does not save the added question automatically. Make sure to enter 2 before closing." << std::endl;
	std::getline(std::cin, filename);

	Quiz quiz;
	// MAIN MENU 
	int i = 0;
	while (i != 9) {
		std::cout << "1: Initialize a file (deletes exisiting file), 2: Save to file, 3: Load from file, 4: Add a question, 5: Play game, 6: Print report, 7: Quit" << std::endl;
		std::cout << "Enter a number:" << std::endl;
		std::cin >> i;

		if (i == 1) {
			initialize( filename);
		}
		if (i == 2) {
			// Save to file
			quiz.serialize(filename);
		}
		if (i == 3) {
			// Read questions from file
			if (quiz.deserialize(filename) >= 0) {
				std::cout << quiz.getSize() << " questions were loaded." << std::endl;
			}
			else { 
				std::cout << "Error occured during file interaction." << std::endl;
			}
		}
		if (i == 4) {
			// Add a new question
			quiz.add();
		}
		if (i == 5) {
			// Take quiz
			int score = quiz.play_game(5);
			std::cout << "Your score:" << score << "out of 5" << std::endl;
		}
		if (i == 6) {
			quiz.print_report();
		}
		// Print report
		if (i == 9){
			//quit();
		}
	}

	return 1;
}
/// <summary>
/// Initialize creates a file if no file exists and deletes the content of exisiting files.
/// </summary>
/// <param name="filename"></param>
void initialize(const std::string &filename) {
	std::ofstream out;
	out.open(filename, std::ofstream::out | std::ofstream::trunc);
	out.close();
}