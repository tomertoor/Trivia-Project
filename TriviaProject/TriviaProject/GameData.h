#pragma once
#include "Question.h"

struct GameData
{
	GameData() {};
	Question currentQuetion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
};