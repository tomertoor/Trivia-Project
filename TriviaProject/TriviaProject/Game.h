#pragma once

#include "Question.h"
#include "LoggedUser.h"
#include "GameData.h"
#include <map>
#include <unordered_map>
#include <cstdio>
#include <ctime>
#include "GameException.h"

class Game
{
private:
	std::vector<Question> m_questions;
	std::unordered_map<LoggedUser, GameData*, UserHash> m_players;
	int m_gameId;
	std::clock_t startTime;
public:
	Game(const std::vector<Question> questions, int gameId);
	~Game() = default;

	std::unordered_map<LoggedUser, GameData*, UserHash> getPlayers();

	Question getQuestionForUser(const LoggedUser& loggedUser);
	void addUser(const LoggedUser& loggedUser);
	void submitAnswer(const LoggedUser& loggedUser, int answerId);
	void removePlayer(const LoggedUser& loggedUser);
	int getId();
};