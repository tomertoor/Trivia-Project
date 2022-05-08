#include "RequestHandlerFactory.h"
#include "MongoDataBase.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase>& database) :
	m_database(database), m_loginManager(&LoginManager::getInstance(m_database))
{
}

RequestHandlerFactory& RequestHandlerFactory::getInstance(std::shared_ptr<IDatabase>& database)
{
	static RequestHandlerFactory instance(database);
	return instance;
}

std::shared_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
	return std::shared_ptr<LoginRequestHandler>(new LoginRequestHandler(m_loginManager, this));
}

std::shared_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler()
{
	return std::shared_ptr<MenuRequestHandler>(new MenuRequestHandler()); // to be changed next version
}
