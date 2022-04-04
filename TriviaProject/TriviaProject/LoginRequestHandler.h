#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestHandler
{
private:
	LoginManager* m_loginManager;
	RequestHandlerFactory* m_handlerFactory;

public:
	bool isRequestRelevant(Requests::RequestInfo request);
	Requests::RequestResult handleRequest(Requests::RequestInfo request);
	Requests::RequestResult login(Requests::RequestInfo loginDetails);
	Requests::RequestResult signup(Requests::RequestInfo registerDetails);

};