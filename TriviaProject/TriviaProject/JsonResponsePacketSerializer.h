#pragma once

#include "Responses.h"
#include "Buffer.h"

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(Responses::ErrorResponse response);
	static Buffer serializeResponse(Responses::LoginResponse response);
	static Buffer serializeResponse(Responses::SignupResponse response);

};