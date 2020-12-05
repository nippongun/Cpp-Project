#pragma once
#include "Question.h"
#include <vector>
class Quiz
{
public:
	Quiz() {};
	void add();
	std::vector<Question> getList() { return quiz_list; };
	int getSize() { return quiz_list.size(); };
	void serialize(const std::string& filename);
	int deserialize(const std::string& filename);
	void print_report();
private:
	std::vector<Question> quiz_list;
};

