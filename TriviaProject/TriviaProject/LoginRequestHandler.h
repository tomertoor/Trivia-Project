#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "MenuRequestHandler.h"
#include <regex>
#include "GlobalException.h"

#define MIN_NAME_LENGTH 5
#define MAX_NAME_LENGTH 15

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
private:
	LoginManager* m_loginManager;
	RequestHandlerFactory* m_handlerFactory;

public:
	LoginRequestHandler();
	LoginRequestHandler(LoginManager* manager, RequestHandlerFactory* factory);
	virtual ~LoginRequestHandler();

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
private:
	Requests::RequestResult login(Requests::LoginRequest loginDetails);
	Requests::RequestResult signup(Requests::SignupRequest registerDetails);
	bool isValidUserName(const std::string& username);
	bool isEmailValid(const std::string& email);
};
