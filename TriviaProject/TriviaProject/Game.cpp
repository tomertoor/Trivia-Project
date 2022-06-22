#include "Game.h"

//constructor
Game::Game(const std::vector<Question> questions, int gameId)
{
	this->m_players = new std::unordered_map<LoggedUser, GameData*, UserHash>();
	for (unsigned int i = 0; i < questions.size(); i++)
		this->m_questions.push_back(questions[i]);
	this->m_gameId = gameId;
	this->startTime = 0;
}

Game::~Game()
{
	delete this->m_players;
}

//Returns the player data
std::unordered_map<LoggedUser, GameData*, UserHash> Game::getPlayers()
{
	return std::unordered_map<LoggedUser, GameData*, UserHash>(*this->m_players);
}

//return the current question for a player
Question Game::getQuestionForUser(const LoggedUser& loggedUser)
{
	this->startTime = std::clock();
	if (this->m_players->find(loggedUser)->second->currentQuestionIndex == this->m_questions.size())
	{
		throw GameException(NO_QUESTIONS);
	}
	return this->m_players->find(loggedUser)->second->currentQuetion;
}

//This function adds a user to the game
void Game::addUser(const LoggedUser& loggedUser)
{
	GameData* gamedata = new GameData(this->m_questions[0]);
	this->m_players->insert(std::pair<LoggedUser, GameData*>(loggedUser, gamedata));
}

//this function adds to the user game data the data given
void Game::submitAnswer(const LoggedUser& loggedUser, int answerId)
{
	double duration = (std::clock() - this->startTime) / (double)CLOCKS_PER_SEC;
	if ((*this->m_players)[loggedUser]->currentQuetion.getCorrectAnswerId() == answerId)
		(*this->m_players)[loggedUser]->correctAnswerCount++;
	else
		(*this->m_players)[loggedUser]->wrongAnswerCount++;
	(*this->m_players)[loggedUser]->averageAnswerTime = (((*this->m_players)[loggedUser]->currentQuestionIndex) * (*this->m_players)[loggedUser]->averageAnswerTime + duration) / ((*this->m_players)[loggedUser]->currentQuestionIndex + 1);
	(*this->m_players)[loggedUser]->currentQuestionIndex++;
	if ((*this->m_players)[loggedUser]->currentQuestionIndex < this->m_questions.size())
	{
		(*this->m_players)[loggedUser]->currentQuetion = this->m_questions[(*this->m_players)[loggedUser]->currentQuestionIndex];
	}
}

//This function removes a player from the players in the game
void Game::removePlayer(const LoggedUser& loggedUser)
{
	delete (*this->m_players)[loggedUser];
	(*this->m_players).erase(loggedUser);
}

//return the game id
int Game::getId()
{
	return this->m_gameId;
}
