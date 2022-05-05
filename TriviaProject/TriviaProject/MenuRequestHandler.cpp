#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler()
{
}

MenuRequestHandler::~MenuRequestHandler()
{
}

bool MenuRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	return false;
}

Requests::RequestResult MenuRequestHandler::handleRequest(Requests::RequestInfo request)
{
	return Requests::RequestResult();
}
