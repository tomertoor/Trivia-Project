#pragma once

#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	Requests::RequestResult leaveRoom(Requests::RequestInfo request);

	Room* m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
	Requests::RequestResult getRoomState(Requests::RequestInfo request);

public:
	RoomMemberRequestHandler(Room* room, LoggedUser user);
	virtual ~RoomMemberRequestHandler() = default;
	virtual bool isRequestRelevant(Requests::RequestInfo request) override;
	virtual Requests::RequestResult handleRequest(Requests::RequestInfo request) override;

};