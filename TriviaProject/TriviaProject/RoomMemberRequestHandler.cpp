#include "RoomMemberRequestHandler.h"


/*Responsible on handling the room state
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomMemberRequestHandler::getRoomState(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	try
	{
		RoomData data = this->m_room->getData();
		std::vector<std::string> users;
		for (auto& it : this->m_room->getAllUsers())
		{
			users.push_back(it.getName());
		}
		Responses::GetRoomStateResponse response = { OK_STATUS, (bool)data.isActive, users, data.numOfQuestionsInGame, data.timePerQuestion };
		if ((bool)data.isActive)
		{
			result.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_user, this->m_handlerFactory.getGameManager().getGameById(this->m_room->getData().id));
		}
		else
		{
			result.newHandler = nullptr;
		}
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		
	}
	catch(std::exception e)
	{
		Responses::LeaveRoomResponse leaveResponse{ OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(leaveResponse);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}
	return result;

	
}

/*Function responsible for handling leave room,
* Input - request: the request info to handle
* Output - The result
*/
Requests::RequestResult RoomMemberRequestHandler::leaveRoom(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	try
	{
		this->m_room->removeUser(this->m_user.getName());
		Responses::LeaveRoomResponse response = { OK_STATUS};
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}
	catch (std::exception e)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory.createMemberRequestHandler(this->m_user, this->m_room);
	}
	return result;
}

//Constructor
RoomMemberRequestHandler::RoomMemberRequestHandler(Room* room, LoggedUser user) : m_room(room), m_user(user), m_handlerFactory(*RequestHandlerFactory::getInstance()), m_roomManager(*RoomManager::getInstance())
{
}


/*Checks if the request id is relevant to the handler functions
* Input - the request
* Output - if its relevant
*/
bool RoomMemberRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	if (request.id == std::atoi(ROOM_STATE_CODE) || request.id == std::atoi(LEAVE_ROOM_CODE))
	{
		return true;
	}
	return false;
}

/*Function responsible on handling request
* Input - request: the info
* Output - the request results
*/
Requests::RequestResult RoomMemberRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	if (request.id == std::atoi(ROOM_STATE_CODE))
	{
		return this->getRoomState(request);
	}
	else if (request.id == std::atoi(LEAVE_ROOM_CODE))
	{
		return this->leaveRoom(request);
	}
	Responses::ErrorResponse errorResponse = { "Error, incorrect state" };
	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = this->m_handlerFactory.createMemberRequestHandler(this->m_user, this->m_room);
	return result;
}
