#include "GameRequestHandler.h"

Requests::RequestResult GameRequestHandler::submitAnswer(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
		this->m_game.submitAnswer(this->m_user, request.answerId);
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

Requests::RequestResult GameRequestHandler::getQuestion(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
		Question question = this->m_game.getQuestionForUser(this->m_user);

		auto answersVec = question.getPossibleAnswers();
		std::map<unsigned int, std::string> answersMap;

		for (int i = 0; i < answersVec.size(); i++)
		{
			answersMap.insert(std::make_pair(i, answersVec[i]));
		}

		Responses::GetQuestionResponse response = { OK_STATUS, question.getQuestion(), answersMap};
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

Requests::RequestResult GameRequestHandler::getGameResults(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		std::map<LoggedUser, GameData> resultsMap = this->m_game.getPlayers();
		std::vector<PlayerResults> resultsVec;
		for (auto& it : resultsMap)
		{
			resultsVec.push_back(PlayerResults{it.first.getName(), it.second.correctAnswerCount, it.second.wrongAnswerCount, it.second.averageAnswerTime});
		}
		Responses::GetGameResultsResponse response = { OK_STATUS, resultsVec};
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

Requests::RequestResult GameRequestHandler::leaveGame(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		this->m_game.removePlayer(this->m_user);
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

GameRequestHandler::GameRequestHandler(LoggedUser user, Game game) : m_user(user), m_game(game), m_handlerFactory(*RequestHandlerFactory::getInstance()), m_gameManager(*GameManager::getInstance())
{
}

bool GameRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	if (request.id == std::atoi(LEAVE_GAME_CODE) || request.id == std::atoi(GET_RESULTS_CODE) || request.id == std::atoi(SUBMIT_ANSWER_CODE) || request.id == std::atoi(GET_QUESTION_CODE))
	{
		return true;
	}
	return false;
}

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
