#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
	m_handlerFactory = new RequestHandlerFactory;
	m_loginManager = new LoginManager;
}

LoginRequestHandler::~LoginRequestHandler()
{
	delete m_handlerFactory;
	delete m_loginManager;
}

bool LoginRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	return false;
}

Requests::RequestResult LoginRequestHandler::handleRequest(Requests::RequestInfo request)
{
	return Requests::RequestResult();
}

Requests::RequestResult LoginRequestHandler::login(Requests::LoginRequest loginDetails)
{

	return 
}

Requests::RequestResult LoginRequestHandler::signup(Requests::SignupRequest registerDetails)
{
	m_loginManager->signup(registerDetails.email, registerDetails.username, registerDetails.password);
	return Requests::RequestResult();
}
