#pragma once

#include "Buffer.h"
#include "IRequestHandler.h"
#include <ctime>
#include "Address.h"

#define SIGNUP_REQUEST 1

#define SIGNIN_REQUEST 5

class IRequestHandler;

namespace Requests
{
	struct RequestResult
	{
		Buffer response;
		IRequestHandler* newHandler;
	};
	struct RequestInfo
	{
		int id;
		tm receivalTime;
		Buffer buffer;
	};
	struct LoginRequest
	{
		std::string username;
		std::string password;
	};
	struct SignupRequest
	{
		std::string username;
		std::string password;
		std::string email;
		Address address;
		std::string phone;
		std::string birthDate;
	};
	struct GetPlayersInRoomRequest
	{
		unsigned int roomId;
	};
	struct JoinRoomRequest
	{
		unsigned int roomId;
	};
	struct CreateRoomRequest
	{
		std::string roomName;
		unsigned int maxUsers;
		unsigned int questionCount;
		unsigned int questionTimeout;
	};
	struct SubmitAnswerRequest
	{
		unsigned int answerId;
	};
};