#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager m_loginManager;
	IDatabase* m_database;
public:
	RequestHandlerFactory();
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();

	LoginManager& getLoginManager();

};