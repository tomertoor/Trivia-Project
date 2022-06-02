#include "RoomAdminRequestHandler.h"


RoomAdminRequestHandler::RoomAdminRequestHandler(Room room, LoggedUser user) :
	m_handlerFactory(RequestHandlerFactory::getInstance()), m_room(room)
{
	m_roomManager = new RoomManager();
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

/*Responsible on handling close room
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomAdminRequestHandler::closeRoom(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{

	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler();
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

	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler();
	}
	return result;
}

/*Responsible on handling the room state
* Input - the request to handle
* Output - the request result
*/
Requests::RequestResult RoomAdminRequestHandler::getRoomState(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{

	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler();
	}
	return result;
}