#pragma once

#include <iostream>

#define WAITING_STATUS 0
#define ONGOING_STATUS 1

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};
