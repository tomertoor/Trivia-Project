#pragma once
#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"
class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
private:

	Room* m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
	Requests::RequestResult getRoomState(Requests::RequestInfo request);
	Requests::RequestResult closeRoom(Requests::RequestInfo info);
	Requests::RequestResult startGame(Requests::RequestInfo info);
public:

	RoomAdminRequestHandler(Room* room, LoggedUser user);
	virtual ~RoomAdminRequestHandler() { };

	bool isRequestRelevant(Requests::RequestInfo request);
	Requests::RequestResult handleRequest(Requests::RequestInfo request);
};