#pragma once
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "StatisticsManager.h"
#include "RoomManager.h"
#include "MenuRequestHandler.h"

#include "RoomMemberRequestHandler.h"
#include "RoomAdminRequestHandler.h"

#include "GameRequestHandler.h"

class MenuRequestHandler;
class LoginRequestHandler;
class RoomMemberRequestHandler;
class RoomAdminRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager* m_loginManager;
	StatisticsManager* m_StatisticsManager;
	RoomManager* m_roomManager;
	GameManager* m_gameManager;

	IDatabase* m_database;

	RequestHandlerFactory();

	static RequestHandlerFactory* instance;

public:

	RequestHandlerFactory(RequestHandlerFactory&) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory&) = delete;

	static RequestHandlerFactory* getInstance();

	~RequestHandlerFactory() = default;

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	RoomMemberRequestHandler* createMemberRequestHandler(LoggedUser user, Room* room);
	RoomAdminRequestHandler* createAdminRequestHandler(LoggedUser user, Room* room);
	GameRequestHandler* createGameRequestHandler(LoggedUser, Game game);

	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	GameManager& getGameManager();


};