#include "RoomAdminRequestHandler.h"


RoomAdminRequestHandler::RoomAdminRequestHandler(Room* room, LoggedUser user) : m_room(room), m_user(user), m_handlerFactory(*RequestHandlerFactory::getInstance()), m_roomManager(*RoomManager::getInstance())
{
}

/*
This function checks if the request is relevant for this handler
input: request info struct
output: true if close room, start game or room state
*/
bool RoomAdminRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	if (request.id == std::atoi(CLOSE_ROOM_CODE) || request.id == std::atoi(START_GAME_CODE) || request.id == std::atoi(ROOM_STATE_CODE))
	{
		return true;
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

	if (request.id == std::atoi(CLOSE_ROOM_CODE))
	{
		return this->closeRoom(request);
	}
	else if (request.id == std::atoi(START_GAME_CODE))
	{
		return this->startGame(request);
	}
	else if (request.id == std::atoi(ROOM_STATE_CODE))
	{
		return this->getRoomState(request);
	}
	return result;
}

Requests::RequestResult RoomAdminRequestHandler::getRoomState(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	try
	{
		RoomData data = this->m_room->getData();
		std::vector<std::string> users;
		auto usersVec = this->m_room->getAllUsers();
		for (unsigned int i = 0; i < usersVec.size(); i++)
		{
			users.push_back(usersVec[i].getName());
		}
		Responses::GetRoomStateResponse response = { OK_STATUS, (bool)data.isActive, users, data.timePerQuestion, data.numOfQuestionsInGame };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr; // to be changed
	}
	catch (std::exception e)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = (IRequestHandler*) this->m_handlerFactory.createMemberRequestHandler(this->m_user, this->m_room);
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
		this->m_roomManager.deleteRoom(this->m_room->getData().id);
		Responses::CloseRoomResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user); // to be changed
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
		this->m_room->startRoom();
		Responses::StartGameResponse response = { OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createGameRequestHandler(this->m_user, this->m_handlerFactory.getGameManager().createGame(*this->m_room)); // to be changed
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	}
	return result;
}
