#include "RoomAdminRequestHandler.h"


RoomAdminRequestHandler::RoomAdminRequestHandler(Room room, LoggedUser user) : m_room(room), m_user(user), m_handlerFactory(*RequestHandlerFactory::getInstance()), m_roomManager(*RoomManager::getInstance())
{
}

/*
This function checks if the request is relevant for this handler
input: request info struct
output: true if close room, start game or room state
*/
bool RoomAdminRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	switch (request.id)
	{
	case CLOSE_ROOM_CODE:
		return true;
	case START_GAME_CODE:
		return true;
	case ROOM_STATE_CODE:
		return true;
	default:
		return false;
	}
	return false;
}

/*Responsible on hnadling request by sending it to private function
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomAdminRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult result;

	switch (request.id)
	{
	case CLOSE_ROOM_CODE:
		return this->closeRoom(request);
	case START_GAME_CODE:
		return this->startGame(request);
	case ROOM_STATE_CODE:
		return this->getRoomState(request);
	default:
		return result;
	}
	return result;
}

Requests::RequestResult RoomAdminRequestHandler::getRoomState(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	try
	{
		RoomData data = this->m_room.getData();
		std::vector<std::string> users;
		for (auto& it : this->m_room.getAllUsers())
		{
			users.push_back(it.getName());
		}
		Responses::GetRoomStateResponse response = { OK_STATUS, data.isActive, users, data.timePerQuestion, data.numOfQuestionsInGame };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr; // to be changed
	}
	catch (std::exception e)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory.createMemberRequestHandler(this->m_user, this->m_room);
	}
	return result;
}

/*Responsible on handling close room
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomAdminRequestHandler::closeRoom(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		this->m_roomManager.deleteRoom(this->m_room.getData().id);
		Responses::CloseRoomResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr; // to be changed
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}

/*Responsible on handling start game
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomAdminRequestHandler::startGame(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		this->m_room.startRoom();
		Responses::StartGameResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr; // to be changed
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}
