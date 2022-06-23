#pragma once
#include "Question.h"

struct GameData
{
	GameData() :
		currentQuetion(), correctAnswerCount(0), wrongAnswerCount(0), averageAnswerTime(0), currentQuestionIndex(0)
	{

	}
	GameData(const Question& ques) :
		currentQuetion(ques), correctAnswerCount(0), wrongAnswerCount(0) , averageAnswerTime(0), currentQuestionIndex(0)
	{ 
	};

	Question currentQuetion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
	unsigned int currentQuestionIndex;
	bool operator==(const GameData& other)
	{
		return averageAnswerTime == other.averageAnswerTime;
	}
};