#pragma once

#include "Buffer.h"
#include "IRequestHandler.h"
#include <ctime>

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
}