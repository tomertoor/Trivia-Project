#pragma once

#include "Game.h"
#include "IDatabase.h"
#include "MongoDataBase.h"
#include "Room.h"

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
	int gameCounter;
	static GameManager* instance;
	GameManager();
public:
	~GameManager() = default;
	GameManager(GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	static GameManager* getInstance();

	Game createGame(Room& room);
	void deleteGame(int gameId);
};