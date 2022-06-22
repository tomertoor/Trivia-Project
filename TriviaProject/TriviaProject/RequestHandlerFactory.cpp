#include "RequestHandlerFactory.h"
#include "MongoDataBase.h"


RequestHandlerFactory* RequestHandlerFactory::instance = nullptr;

RequestHandlerFactory::RequestHandlerFactory()
{
	m_loginManager = LoginManager::getInstance();
	m_database = MongoDataBase::getInstance();
	m_StatisticsManager = StatisticsManager::getInstance();
	m_roomManager = RoomManager::getInstance();
	m_gameManager = GameManager::getInstance();
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

GameManager& RequestHandlerFactory::getGameManager()
{
	return *this->m_gameManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	return new MenuRequestHandler(user); 
}

RoomMemberRequestHandler* RequestHandlerFactory::createMemberRequestHandler(LoggedUser user, Room* room)
{
	return new RoomMemberRequestHandler(room, user);
}

RoomAdminRequestHandler* RequestHandlerFactory::createAdminRequestHandler(LoggedUser user, Room* room)
{
	return new RoomAdminRequestHandler(room, user);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Game* game)
{
	return new GameRequestHandler(user, game);
}
