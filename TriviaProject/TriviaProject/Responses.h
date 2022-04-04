#pragma once

#include <iostream>

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