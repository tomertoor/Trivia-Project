#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory()
{
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
