#include "Question.h"

//default constrcutor
Question::Question() :
	m_question(""), m_correctAnswerId(0)
{
}

//constructor
Question::Question(const std::string& question, const std::vector<std::string>& possibleAnswers, int correctAnswerId)
{
	this->m_correctAnswerId = correctAnswerId;
	this->m_question = question;
	for (unsigned int i = 0; i < possibleAnswers.size(); i++)
		this->m_possibleAnswers.push_back(possibleAnswers[i]);
}

/*
This function returns the question
input: none
output: the question
*/
std::string Question::getQuestion()
{
	return this->m_question;
}

/*
This function returns a vector of the possibble answers
input: none
output: the vector of answers
*/
std::vector<std::string> Question::getPossibleAnswers()
{
	std::vector<std::string> possibleAnswers;
	for (unsigned int i = 0; i < this->m_possibleAnswers.size(); i++)
		possibleAnswers.push_back(this->m_possibleAnswers[i]);
	return possibleAnswers;
}

/*
This function returns the correct answer id
input: none
output: the correct answer id
*/
int Question::getCorrectAnswerId()
{
	return this->m_correctAnswerId;
}
