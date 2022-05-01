#pragma once

#include "Responses.h"
#include "Buffer.h"
#include "json.hpp"

using json = nlohmann::json;

#define MAX_FILLING_LENGTH 4
#define ERROR_CODE 3
#define LOGIN_CODE 5
#define SIGNUP_CODE 1

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(Responses::ErrorResponse response);
	static Buffer serializeResponse(Responses::LoginResponse response);
	static Buffer serializeResponse(Responses::SignupResponse response);

};