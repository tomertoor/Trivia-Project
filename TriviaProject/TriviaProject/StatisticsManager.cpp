#include "StatisticsManager.h"

StatisticsManager* StatisticsManager::instance = nullptr;

/*Sends the top 5 highest scores
* Input - None
* Output - vector with 5 highest scores (if there is less then the x highest scores)
*/
std::vector<std::string> StatisticsManager::getHighScore()
{
	return this->m_database->getHighestScores();
}

/*Gets the user statistics, fetches from the db and inserts it to vector
* Input - username, the username to look for
* Output - the stats
*/
std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
	std::vector<std::string> result;
	result.push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
	result.push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));
	result.push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
	result.push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username)));

	return result;
}

StatisticsManager* StatisticsManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new StatisticsManager();
	}
	return instance;
}
