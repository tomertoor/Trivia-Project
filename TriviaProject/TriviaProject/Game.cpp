#include "Game.h"

//constructor
Game::Game(const std::vector<Question> questions, int gameId)
{
	for (int i = 0; i < questions.size(); i++)
		this->m_questions.push_back(questions[i]);
	this->m_gameId = gameId;
}

//Returns the player data
std::map<LoggedUser, GameData> Game::getPlayers()
{
	return std::map<LoggedUser, GameData>(this->m_players);
}

//return the current question for a player
Question Game::getQuestionForUser(const LoggedUser& loggedUser)
{
	this->startTime = std::clock();
	
	return Question("asd", {"asd"}, 5);//this->m_players.find(loggedUser)->second.currentQuetion;
}

//This function adds a user to the game
void Game::addUser(const LoggedUser& loggedUser)
{
	GameData gamedata(this->m_questions[0]);
	this->m_players.insert(std::pair<LoggedUser, GameData>(loggedUser, gamedata));
}

//this function adds to the user game data the data given
void Game::submitAnswer(const LoggedUser& loggedUser, int answerId)
{
	double duration = (std::clock() - this->startTime) / (double)CLOCKS_PER_SEC;
	if (this->m_players[loggedUser].currentQuetion.getCorrectAnswerId() == answerId)
		this->m_players[loggedUser].correctAnswerCount++;
	else
		this->m_players[loggedUser].wrongAnswerCount++;
	this->m_players[loggedUser].averageAnswerTime = ((this->m_players[loggedUser].currentQuestionIndex) * this->m_players[loggedUser].averageAnswerTime + duration) / this->m_players[loggedUser].currentQuestionIndex + 1;
	this->m_players[loggedUser].currentQuestionIndex++;
	this->m_players[loggedUser].currentQuetion = this->m_questions[this->m_players[loggedUser].currentQuestionIndex];


}

//This function removes a player from the players in the game
void Game::removePlayer(const LoggedUser& loggedUser)
{
	//this->m_players.erase(loggedUser);
}

//return the game id
int Game::getId()
{
	return this->m_gameId;
}
