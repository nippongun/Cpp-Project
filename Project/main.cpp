/*
* C++ project "Quiz" by Simon Bauer, 2020, for the Metropolia UAS course "Object oriented programming in IoT"
* 
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
void deserializer(std::vector<Question> vector,const std::string &filename);
void print_report(const std::vector<Question> vector);
int main() {
	std::string filename = FILENAME;
	std::ifstream in(filename);

	Quiz quiz;

	// Deserialize the Vector using istream_iterators 
	//std::vector<Question> v((std::istream_iterator<Question>(in)), std::istream_iterator<Question>());
	//std::vector<Question> quiz_list;
	//std::copy(std::istream_iterator<Question>(in), std::istream_iterator<Question>(), std::back_inserter(quiz_list));
	
	// MAIN MENU 
	int i = 0;
	while (i != 9) {
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
			quiz.deserialize(filename);
			std::cout << quiz.getSize() << " questions were loaded." << std::endl;
		}
		if (i == 4) {
			// Add a new question
			quiz.add();
		}
		if (i == 5) {
			// Take quiz
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

void initialize(const std::string &filename) {
	std::ofstream out;
	out.open(filename, std::ofstream::out | std::ofstream::trunc);
	out.close();
}

void deserializer(std::vector<Question> vector,const std::string &filename ){
	std::ofstream out(filename);
	std::for_each(vector.begin(), vector.end(), [&out](Question q) {
		out << q;
	});
	out.close();
}

void print_report(const std::vector<Question> vector) {
	int i = 1;
	std::for_each(vector.begin(), vector.end(), [&i](Question q) {
		std::cout << "Question " << i << ": "<< q.getQuestion() << std::endl;
		++i;
	});
}