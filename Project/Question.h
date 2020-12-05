#pragma once
#include <string>
#include <iostream>
#include <ostream>

class Question
{
	friend std::istream &operator>>(std::istream& is,Question &q);
	friend std::ostream& operator<<(std::ostream& os,const Question& q);
public:
	Question() { question = ""; alternative1 = ""; alternative2 = ""; alternative3 = ""; alternative4 = ""; answer = 0; };
	~Question() = default;
	std::string getQuestion() { return question; };
	int getAnswer() { return answer; };
	void input();
private:
	std::string question;
	std::string alternative1;
	std::string alternative2;
	std::string alternative3;
	std::string alternative4;
	int answer;
};

