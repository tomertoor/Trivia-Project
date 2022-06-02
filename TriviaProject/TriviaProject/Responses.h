#pragma once

#include <iostream>
#include <vector>
#include "RoomData.h"

#define OK_STATUS 1

namespace Responses
{
	struct LoginResponse
	{
		unsigned int status;
	};

	struct SignupResponse
	{
		unsigned int status;
	};
	struct ErrorResponse
	{
		std::string message;
	};
	struct LogoutResponse
	{
		unsigned int status;
	};
	struct GetRoomsResponse
	{
		unsigned int status;
		std::vector<RoomData> rooms;
	};
	struct GetPlayersInRoomResponse
	{
		std::vector<std::string> players;
	};
	struct GetHighScoreResponse
	{
		unsigned int status;
		std::vector<std::string> statistics;
	};
	struct GetPersonalStatsResponse
	{
		unsigned int status;
		std::vector<std::string> statistics;
	};
	struct JoinRoomResponse
	{
		unsigned int status;
	};
	struct CreateRoomResponse
	{
		unsigned int status;
	};
	struct CloseRoomResponse
	{
		unsigned int status;
	};
	struct StartGameResponse
	{
		unsigned int status;
	};
	struct GetRoomStateResponse
	{
		unsigned int status;
		bool hasGameBegun;
		std::vector<std::string> players;
		unsigned int questionCount;
		unsigned int answerTimeout;
	};
	struct LeaveRoomResponse
	{
		unsigned int status;
	};
}