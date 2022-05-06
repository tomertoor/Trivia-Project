#include "RequestHandlerFactory.h"
#include "MongoDataBase.h"

RequestHandlerFactory::RequestHandlerFactory()
{
	m_database = MongoDataBase::getInstance();
}

RequestHandlerFactory::~RequestHandlerFactory()
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(&this->m_loginManager, this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler(); // to be changed next version
}
