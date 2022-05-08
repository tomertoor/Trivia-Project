#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager* m_loginManager;
	std::shared_ptr<IDatabase> m_database;
	RequestHandlerFactory(std::shared_ptr<IDatabase>& database);
public:
	RequestHandlerFactory(const RequestHandlerFactory&) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory&) = delete;
	static RequestHandlerFactory& getInstance(std::shared_ptr<IDatabase>& database);
	~RequestHandlerFactory() = default;

	std::shared_ptr<IRequestHandler> createLoginRequestHandler();
	std::shared_ptr<IRequestHandler> createMenuRequestHandler();

};