#pragma once

#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
private:
	Game* m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	Requests::RequestResult submitAnswer(Requests::RequestInfo info);
	Requests::RequestResult getQuestion(Requests::RequestInfo info);
	Requests::RequestResult getGameResults(Requests::RequestInfo info);
	Requests::RequestResult leaveGame(Requests::RequestInfo info);

public:
	GameRequestHandler(LoggedUser user, Game* game);
	virtual ~GameRequestHandler() = default;

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
};