#include "Quiz.h"
#include "Question.h"
#include <iostream>
#include <fstream>
#include <algorithm>
void Quiz::add(){
	std::cin.ignore();
	Question q;
	q.input();
	quiz_list.push_back(q);
}

int Quiz::deserialize(const std::string &filename) {
	std::ifstream in(filename);
	std::copy(std::istream_iterator<Question>(in), std::istream_iterator<Question>(), std::back_inserter(quiz_list));
	in.close();
	return quiz_list.size();
}

void Quiz::serialize(const std::string& filename) {
	std::ofstream out(filename);
	std::for_each(quiz_list.begin(), quiz_list.end(), [&out](Question q) {
		out << q;
		});
	out.close();
}

void Quiz::print_report() {
	int i = 1;
	std::for_each(quiz_list.begin(), quiz_list.end(), [&i](Question q) {
		std::cout << "Question " << i << ": " << q.getQuestion() << std::endl;
		++i;
	});
}