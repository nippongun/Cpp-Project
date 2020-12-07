#include "Quiz.h"
#include "Question.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
void Quiz::add(){
	std::cin.ignore();
	Question q;
	q.input();
	quiz_list.push_back(q);
}

// deserialize first deletes the current the vector and then loads the data from the file. The purpose is to keep the reference to the file only 
// during this scope.

/// <summary>
/// Takes a string to a open a file and stores the files data into a vector. Returns from 0 if successful, -1 if fail.
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
int Quiz::deserialize(const std::string &filename) {
	quiz_list.clear();
	std::ifstream in(filename);
	// Error cheking
	if (!in.fail()) {
		std::copy(std::istream_iterator<Question>(in), std::istream_iterator<Question>(), std::back_inserter(quiz_list));
	}
	else {
		// 
		return -1;
	}
	in.close();
	return quiz_list.size();
}

/// <summary>
/// Takes a string to a open a file and stores the vector to the file.
/// </summary>
/// <param name="filename"></param>
void Quiz::serialize(const std::string& filename) {
	std::ofstream out(filename);
	std::for_each(quiz_list.begin(), quiz_list.end(), [&out](Question q) {
		out << q;
		});
	out.close();
}
/// <summary>
/// Prints a report of the quiz. Only prints the questions.
/// </summary>
void Quiz::print_report() {
	int i = 1;
	std::for_each(quiz_list.begin(), quiz_list.end(), [&i](Question q) {
		std::cout << "Question " << i << ": " << q.getQuestion() << std::endl;
		++i;
	});
}
/// <summary>
/// play_game picks a given amount of questions, default = 5, and chooses for all questions 3 alternatives.
/// </summary>
/// <param name="amount"></param>
/// <returns>score</returns>
int Quiz::play_game(int amount = 5) {

	int score = 0;
	std::vector<Question> game_deck;
	/*
	*	sample is a part of <algorithm> and folows the C++ 17 standard. It picks a specific amount of data from a set without repitition. It requires a random generator engine to function i.e. the mersenne-twister engine in this case
	*	https://en.cppreference.com/w/cpp/algorithm/sample
	*	https://en.wikipedia.org/wiki/Mersenne_Twister
	*/
	std::sample(quiz_list.begin(), quiz_list.end(), std::back_inserter(game_deck), amount, std::mt19937{ std::random_device{}() }); // Game deck has been defined.
	std::shuffle(game_deck.begin(), game_deck.end(), std::default_random_engine(std::mt19937{ std::random_device{}() }));
	
	for (auto i : game_deck) {
		std::cout << i.getQuestion();
	}
	for (int i = 0; i < amount; i++) {
		std::srand(NULL);
		std::vector<std::string> temp;
		std::string answer;

		if (game_deck[i].getAnswer() == 1) {
			answer = game_deck[i].getAlternative1();
		}
		if (game_deck[i].getAnswer() == 2) {
			answer = game_deck[i].getAlternative2();
		}
		if (game_deck[i].getAnswer() == 3) {
			answer = game_deck[i].getAlternative3();
		}
		if (game_deck[i].getAnswer() == 4) {
			answer = game_deck[i].getAlternative4();
		}

		// excludes ranges from 1 to 4 and excludes an alternative from the beeing picked. However, it is made sure that the answer is given.
		int exclude;
		do {
			exclude = rand() % 4 + 1;
		} while (exclude == game_deck[i].getAnswer());
	
		// Depending on the excluded alternative, the other are getting assigned.
		if (exclude == 1) {
			temp.push_back(game_deck[i].getAlternative2());
			temp.push_back(game_deck[i].getAlternative3());
			temp.push_back(game_deck[i].getAlternative4());
		}
		if (exclude == 2) {
			temp.push_back(game_deck[i].getAlternative1());
			temp.push_back(game_deck[i].getAlternative3());
			temp.push_back(game_deck[i].getAlternative4());
		}
		if (exclude == 3) {
			temp.push_back(game_deck[i].getAlternative1());
			temp.push_back(game_deck[i].getAlternative2());
			temp.push_back(game_deck[i].getAlternative4());
		}
		if (exclude == 4) {
			temp.push_back(game_deck[i].getAlternative1());
			temp.push_back(game_deck[i].getAlternative2());
			temp.push_back(game_deck[i].getAlternative3());
		}

		std::shuffle(temp.begin(), temp.end(), std::default_random_engine(std::mt19937{ std::random_device{}() })); // shuffle the temporary vector.

		// Printing of the questions
		int answer_counter = 1;
		int selection = 0;
		std::cout << "Question " << i + 1 << ": " << game_deck[i].getQuestion() << std::endl;
		std::for_each(temp.begin(), temp.end(), [&answer_counter](std::string s) { 
			std::cout << "Answer " << answer_counter << ": " << s << std::endl; 
			answer_counter++;
			});

		// Keep track of the score
		std::cin >> selection;
		if (temp[selection-1] != answer) {
			std::cout << "The answer is wrong!" << std::endl;
		}
		else if (temp[selection-1] == answer) {
			score++;
			std::cout << "The answer is right!" << std::endl;
		}
	}
	return score;
}