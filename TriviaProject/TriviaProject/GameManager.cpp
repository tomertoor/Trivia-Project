#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() :
	m_database(MongoDataBase::getInstance()), gameCounter(1)
{
}

GameManager* GameManager::getInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	return instance;
}

Game GameManager::createGame(Room& room)
{
	std::vector<Question> questions;
	for (auto& question : this->m_database->getQuestions(room.getData().numOfQuestionsInGame))
		questions.push_back(question);
	Game game(questions, gameCounter);
	for (int i = 0; i < room.getAllUsers().size(); i++)
		game.addUser(room.getAllUsers()[i]);
	this->m_games.push_back(game);
	return game;
}

void GameManager::deleteGame(int gameId)
{
	for (auto& game : this->m_games)
	{
		if (game.getId() == gameId)
			this->m_games.erase(game);
	}
}
