#pragma once

#include <iostream>
#include "MongoDataBase.h"

class StatisticsManager
{
public:
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
	static StatisticsManager* getInstance();
private:
	
	MongoDataBase* m_database;
	static StatisticsManager* instance;

};