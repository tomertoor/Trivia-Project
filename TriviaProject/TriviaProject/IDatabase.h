#pragma once
#include <string>
#include <vector>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email,
		const std::string& phone, const std::string& birthDate,
		const std::string& apt, const std::string& city, const std::string& street) = 0;

	virtual float getPlayerAverageAnswerTime(std::string name) = 0;
	virtual int getNumOfCorrectAnswers(std::string name) = 0;
	virtual int getNumOfTotalAnswers(std::string name) = 0;
	virtual int getNumOfPlayerGames(std::string name) = 0;

	virtual std::vector<int> getHighestScores(std::string name) = 0;
};