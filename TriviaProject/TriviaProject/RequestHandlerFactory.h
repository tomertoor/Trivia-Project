#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
private:
	LoginManager m_loginManager;
	IDatabase* m_database;
public:
	RequestHandlerFactory();
	~RequestHandlerFactory();

	LoginRequestHandler createLoginRequestHandler();
};