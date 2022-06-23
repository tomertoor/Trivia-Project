#include "GameRequestHandler.h"

/*Function responsible on submitting answer
* Input - info: the info
* Output - The result if submit was ok
*/
Requests::RequestResult GameRequestHandler::submitAnswer(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
		this->m_game->submitAnswer(this->m_user, request.answerId);
		Responses::SubmitAnswerResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}

/*Function responsible on getting question
* Input - the info to handle
* Output - the result
*/
Requests::RequestResult GameRequestHandler::getQuestion(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Question question = this->m_game->getQuestionForUser(this->m_user);

		auto answersVec = question.getPossibleAnswers();
		std::map<unsigned int, std::string> answersMap;

		for (unsigned int i = 0; i < answersVec.size(); i++)
		{
			answersMap.insert(std::make_pair(i, answersVec[i]));
		}

		Responses::GetQuestionResponse response = { OK_STATUS, question.getQuestion(), answersMap};
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (GameException ex)
	{
		Responses::GetQuestionResponse response = { ex.status, "", {} };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;

	}
	return result;
}

/*Function responsible on getting game results
* Input - The info to handle
* Output - the result of the request
*/
Requests::RequestResult GameRequestHandler::getGameResults(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		std::unordered_map<LoggedUser, GameData*, UserHash> resultsMap = this->m_game->getPlayers();
		std::vector<LoggedUser> users;
		for (auto& it : resultsMap)
		{
			users.push_back(it.first);
		}
		std::vector<PlayerResults> resultsVec;
		for (unsigned int i = 0; i < users.size(); i++)
		{
			resultsVec.push_back(PlayerResults{users[i].getName(), resultsMap[users[i]]->correctAnswerCount, resultsMap[users[i]]->wrongAnswerCount, resultsMap[users[i]]->averageAnswerTime});
		}
		Responses::GetGameResultsResponse response = { OK_STATUS, resultsVec};
		this->m_handlerFactory.getGameManager().updateStatistics(this->m_user.getName(), *resultsMap[this->m_user]);
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		try
		{
			this->m_handlerFactory.getRoomManager().getRooms().at(this->m_game->getId()); // if the room doesnt exist it will trigger expection
			this->m_handlerFactory.getRoomManager().deleteRoom(this->m_game->getId());
		}
		catch(...)
		{
		}
		try
		{
			this->m_handlerFactory.getGameManager().getGames().at(this->m_game->getId()); // if the game doesnt exist it will trigger expection
			this->m_handlerFactory.getGameManager().deleteGame(this->m_game->getId());
		}
		catch (...)
		{
		}
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}
	catch (std::exception e)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour. " + std::string(e.what()) };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;

	}
	return result;
}
/*Responsible on leaving game
* Input - the info
* Output - the result
*/
Requests::RequestResult GameRequestHandler::leaveGame(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		this->m_game->removePlayer(this->m_user);
		Responses::LeaveGameResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;

	}
	return result;
}

GameRequestHandler::GameRequestHandler(LoggedUser user, Game* game) : m_user(user), m_game(game), m_handlerFactory(*RequestHandlerFactory::getInstance()), m_gameManager(*GameManager::getInstance())
{
}


/*
This function checks if the request is relevant for this handler
input: request info struct
output: true if close room, start game or room state
*/
bool GameRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	if (request.id == std::atoi(LEAVE_GAME_CODE) || request.id == std::atoi(GET_RESULTS_CODE) || request.id == std::atoi(SUBMIT_ANSWER_CODE) || request.id == std::atoi(GET_QUESTION_CODE))
	{
		return true;
	}
	return false;
}

/*Responsible on hnadling request by sending it to private function
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult GameRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult result;

	if (request.id == std::atoi(LEAVE_GAME_CODE))
	{
		return this->leaveGame(request);
	}
	else if (request.id == std::atoi(GET_RESULTS_CODE))
	{
		return this->getGameResults(request);
	}
	else if (request.id == std::atoi(SUBMIT_ANSWER_CODE))
	{
		return this->submitAnswer(request);
	}
	else if (request.id == std::atoi(GET_QUESTION_CODE))
	{
		return this->getQuestion(request);
	}
	return result;
}
