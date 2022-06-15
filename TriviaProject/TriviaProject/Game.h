#pragma once

#include "Question.h"
#include "LoggedUser.h"
#include "GameData.h"
#include <map>

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	int m_gameId;
public:
	Game(const std::vector<Question> questions, int gameId);
	~Game() = default;

	Question getQuestionForUser(const LoggedUser& loggedUser);
	void addUser(const LoggedUser& loggedUser);
	void submitAnswer(const LoggedUser& loggedUser, int answerId, float avg);
	void removePlayer(const LoggedUser& loggedUser);
	int getId();
};