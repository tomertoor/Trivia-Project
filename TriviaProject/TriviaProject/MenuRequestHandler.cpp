#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler()
{
}

MenuRequestHandler::~MenuRequestHandler()
{
}

/*Checks if the request is relevant
* Input - request: the request info
* Output - if its relevant or not.
*/
bool MenuRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	switch (request.id)
	{
	case CREATE_ROOM_CODE:
	case GET_PLAYERS_CODE:
	case GET_ROOM_CODE:
	case JOIN_ROOM_CODE:
	case LOGOUT_CODE:
	case STATISTICS_CODE:
		return true;
		break;
	default:
		return false;
	}
	return false;
}

Requests::RequestResult MenuRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult	result;

	switch (request.id)
	{
	case CREATE_ROOM_CODE:
		return this->createRoom(request);
		break;
	case GET_PLAYERS_CODE:
		return this->getPlayersInRoom(request);
		break;
	case GET_ROOM_CODE:
		return this->getRooms(request);
		break;
	case JOIN_ROOM_CODE:
		return this->joinRoom(request);
		break;
	case LOGOUT_CODE:
		return this->signout(request);
		break;
	case STATISTICS_CODE:
		return this->getPersonalStats(request);
		break;
	default:
	}
	return Requests::RequestResult();
}

Requests::RequestResult MenuRequestHandler::signout(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		auto vec = this->m_roomManager.getRooms();
		for (auto& iter : vec)
		{
			iter.removeUser(this->m_user);
		}
		Responses::LogoutResponse response;
		response.status = OK_STATUS;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	return result;
}

Requests::RequestResult MenuRequestHandler::getRooms(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);

		Responses::GetRoomsResponse response;
		response.rooms = this->m_roomManager.getRoomsData();
		response.status = OK_STATUS;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	return result;
}

Requests::RequestResult MenuRequestHandler::getPlayersInRoom(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer);
		
		Responses::GetPlayersInRoomResponse response;
		auto userVec = this->m_roomManager.getRooms()[request.roomId].getAllUsers();
		for (auto& iter : userVec)
		{
			response.players.push_back(iter.getName());
		}

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	return result;
}

Requests::RequestResult MenuRequestHandler::getPersonalStats(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
		this->m_roomManager.createRoom(this->m_user, { 0, request.roomName, request.maxUsers, request.questionCount, request.questionTimeout });

		Responses::CreateRoomResponse response;
		response.status = OK_STATUS;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
}

Requests::RequestResult MenuRequestHandler::getHighScore(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{


		Responses::GetHighScoreResponse response;
		response.statistics = this->m_statisticsManager.getHighScore();

		response.status = OK_STATUS;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
}

Requests::RequestResult MenuRequestHandler::joinRoom(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
		this->m_roomManager.getRooms()[request.roomId].addUser(this->m_user);

		Responses::CreateRoomResponse response;
		response.status = OK_STATUS;

		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
}

Requests::RequestResult MenuRequestHandler::createRoom(Requests::RequestInfo info)
{
	Requests::RequestResult result;

	try
	{
		Requests::CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
		this->m_roomManager.createRoom(this->m_user, { this->m_roomManager.getCurrentId(), request.roomName, request.maxUsers, request.questionCount, request.questionTimeout});
		
		Responses::CreateRoomResponse response;
		response.status = OK_STATUS;
		
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr;
	}
	catch(...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
	return result;
}
