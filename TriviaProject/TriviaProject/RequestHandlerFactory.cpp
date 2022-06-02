#include "RequestHandlerFactory.h"
#include "MongoDataBase.h"


RequestHandlerFactory* RequestHandlerFactory::instance = nullptr;

RequestHandlerFactory::RequestHandlerFactory()
{
	m_loginManager = LoginManager::getInstance();
	m_database = MongoDataBase::getInstance();
}

//get instance for singleton
RequestHandlerFactory* RequestHandlerFactory::getInstance()
{
	if (instance == nullptr)
	{
		instance = new RequestHandlerFactory();
	}
	return instance;
}


LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this->m_loginManager, this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return *this->m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return *this->m_StatisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return *this->m_roomManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler(); // to be changed next version
}
