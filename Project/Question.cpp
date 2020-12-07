#include "Question.h"
#include <iostream>
#include <string>

std::istream& operator>>(std::istream& is, Question& q) {
	std::getline(is,q.question);
	std::getline(is,q.alternative1);
	std::getline(is, q.alternative2);
	std::getline(is, q.alternative3);
	std::getline(is, q.alternative4);
	is >> q.answer;
	is.ignore();
	return is;
}

// The delimiter for each value is \n
std::ostream& operator<<(std::ostream& os, const Question& q) {
	os << q.question;
	os << std::endl;
	os << q.alternative1;
	os << std::endl;
	os << q.alternative2;
	os << std::endl;
	os << q.alternative3;
	os << std::endl;
	os << q.alternative4;
	os << std::endl;
	os << q.answer;
	os << std::endl;
	return os;
}
// Seperate function for the user interaction
void Question::input() {
	std::cout << "Enter the question:" << std::endl;
	std::getline(std::cin, question);
	std::cout << "Enter the first alternative:" << std::endl;
	std::getline(std::cin, alternative1);
	std::cout << "Enter the second alternative:" << std::endl;
	std::getline(std::cin, alternative2);
	std::cout << "Enter the third alternative:" << std::endl;
	std::getline(std::cin, alternative3);
	std::cout << "Enter the fourth alternative:" << std::endl;
	std::getline(std::cin, alternative4);
	while (answer <= 0 || answer > 4) {
		std::cout << "Which alternative is the answer?" << std::endl;
		std::cin >> answer;
	}
}