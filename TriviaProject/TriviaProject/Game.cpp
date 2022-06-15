#include "Game.h"

Game::Game(const std::vector<Question> questions, int gameId)
{
	for (int i = 0; i < questions.size(); i++)
		this->m_questions.push_back(questions[i]);
	this->m_gameId = gameId;
}

Question Game::getQuestionForUser(const LoggedUser& loggedUser)
{
	return this->m_players[loggedUser].currentQuetion;
}

void Game::addUser(const LoggedUser& loggedUser)
{
	GameData gamedata;
	this->m_players.insert(std::pair<LoggedUser, GameData>(loggedUser, gamedata));
}

void Game::submitAnswer(const LoggedUser& loggedUser, int answerId)
{
	if (this->m_players[loggedUser].currentQuetion.getCorrectAnswerId() == answerId)
		this->m_players[loggedUser].correctAnswerCount++;
	else
		this->m_players[loggedUser].wrongAnswerCount++;
	//add the average time
}

void Game::removePlayer(const LoggedUser& loggedUser)
{
	this->m_players.erase(loggedUser);
}

int Game::getId()
{
	return this->m_gameId;
}
