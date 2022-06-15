#pragma once
#include "Question.h"

struct GameData
{
	GameData(const Question& ques) :
		currentQuetion(ques), correctAnswerCount(0), wrongAnswerCount(0) , averageAnswerTime(0), currentQuestionIndex(0)
	{ 
	};

	Question currentQuetion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	int currentQuestionIndex;
};