#pragma once

#include "Question.h"
#include "LoggedUser.h"
#include "GameData.h"
#include <map>
#include <cstdio>
#include <ctime>

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	int m_gameId;
	std::clock_t startTime;
public:
	Game(const std::vector<Question> questions, int gameId);
	~Game() = default;

	std::map<LoggedUser, GameData> getPlayerData();

	Question getQuestionForUser(const LoggedUser& loggedUser);
	void addUser(const LoggedUser& loggedUser);
	void submitAnswer(const LoggedUser& loggedUser, int answerId);
	void removePlayer(const LoggedUser& loggedUser);
	int getId();
};