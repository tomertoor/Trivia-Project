#pragma once

#include <iostream>

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
}