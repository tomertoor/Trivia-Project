#pragma once
#include "IRequestHandler.h"
#include "Requests.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_handlerFactory;

	Requests::RequestResult closeRoom(Requests::RequestInfo info);
	Requests::RequestResult startGame(Requests::RequestInfo info);
	Requests::RequestResult getRoomState(Requests::RequestInfo info);

public:
	RoomAdminRequestHandler(Room room, LoggedUser user);
	virtual ~RoomAdminRequestHandler() { delete m_roomManager; };

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
};