#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	return false;
}

Requests::RequestResult LoginRequestHandler::handleRequest(Requests::RequestInfo request)
{
	return Requests::RequestResult();
}
