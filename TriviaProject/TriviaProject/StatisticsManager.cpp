#include "StatisticsManager.h"

StatisticsManager* StatisticsManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new StatisticsManager();
	}
	return instance;
}
