#include "GameManager.h"
#include "MongoDataBase.h"

GameManager* GameManager::instance = nullptr;

//constructor
GameManager::GameManager() :
	m_database(MongoDataBase::getInstance()), gameCounter(1)
{
}

//get instance for singleton
GameManager* GameManager::getInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	return instance;
}

//this function creates a new game by the room data and adds it to the vector of games
Game* GameManager::createGame(Room& room, const unsigned int& id)
{
	//this->m_database->testQuestions();
	std::vector<Question> questions;
	for (auto& question : this->m_database->getQuestions(room.getData().numOfQuestionsInGame))
		questions.push_back(question);
	Game* game = new Game(questions, id);
	for (int i = 0; i < room.getAllUsers().size(); i++)
		game->addUser(room.getAllUsers()[i]);
	this->m_games.push_back(game);
	return game;
}

//this function removes a game from the vector of games
void GameManager::deleteGame(int gameId)
{
	for (auto it = this->m_games.begin() ; it != this->m_games.end() ; it++)
	{
		if ((*it)->getId() == gameId)
			delete (*it);
			this->m_games.erase(it);
	}
}
