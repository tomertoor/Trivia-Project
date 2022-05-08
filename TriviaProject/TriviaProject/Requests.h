#pragma once

#include "Buffer.h"
#include "IRequestHandler.h"
#include <ctime>

#define SIGNUP_REQUEST 1

#define SIGNIN_REQUEST 5

class IRequestHandler;

namespace Requests
{
	struct RequestResult
	{
		Buffer response;
		std::shared_ptr<IRequestHandler> newHandler;
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
	};
};
