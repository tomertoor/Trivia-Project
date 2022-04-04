#pragma once

#include "Requests.h"

struct Requests::RequestInfo;
struct Requests::RequestResult;

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(Requests::RequestInfo request) = 0;
	virtual Requests::RequestResult handleRequest(Requests::RequestInfo request) = 0;
};