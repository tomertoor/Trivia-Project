#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RequestHandlerFactory* m_handlerFactory;

public:
	LoginRequestHandler();
	~LoginRequestHandler();

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
	Requests::RequestResult login(Requests::LoginRequest loginDetails);
	Requests::RequestResult signup(Requests::SignupRequest registerDetails);

};