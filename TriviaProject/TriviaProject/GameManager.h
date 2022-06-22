#pragma once

#include "Game.h"
#include "IDatabase.h"
#include "Room.h"

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game*> m_games;
	int gameCounter;
	static GameManager* instance;
	GameManager();
public:
	~GameManager() = default;
	GameManager(GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	static GameManager* getInstance();

	Game* createGame(Room& room, const unsigned int& id);
	void deleteGame(int gameId);
	Game* getGameById(const unsigned int& id);
	std::vector<Game*> getGames();

	void updateStatistics(std::string name, GameData results);
};