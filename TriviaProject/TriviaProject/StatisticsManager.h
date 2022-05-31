#pragma once

#include <iostream>
#include "IDatabase.h"

class StatisticsManager
{
public:
	StatisticsManager();
	~StatisticsManager() = default;

	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
	static StatisticsManager* getInstance();
private:
	std::shared_ptr<IDatabase> m_database;
	static StatisticsManager* instance;
};