﻿#pragma once
#include <string>
#include <vector>
#include <list>
#include "Question.h"
#include "GameData.h"

class IDatabase
{
public:

	virtual void testQuestions() = 0;

	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email,
		const std::string& phone, const std::string& birthDate,
		const std::string& apt, const std::string& city, const std::string& street) = 0;

	virtual double getPlayerAverageAnswerTime(std::string name)  = 0;
	virtual int getNumOfCorrectAnswers(std::string name) = 0;
	virtual int getNumOfTotalAnswers(std::string name)  = 0;
	virtual int getNumOfPlayerGames(std::string name)  = 0;

	virtual std::vector<std::string> getHighestScores() = 0;
	virtual void addQuestion(Question& question) = 0;
	virtual std::list<Question> getQuestions(int amount) = 0;
	
	virtual void updateStatistics(const std::string& name, GameData newResults) = 0;
};