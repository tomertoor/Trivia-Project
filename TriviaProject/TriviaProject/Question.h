#pragma once

#include <iostream>
#include <vector>

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswerId;
public:
	Question() = default;
	Question(const std::string& question, const std::vector<std::string>& possibleAnswers, int correctAnswerId);
	~Question() = default;

	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	int getCorrectAnswerId();
};