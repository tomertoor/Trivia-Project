#pragma once

#include "IRequestHandler.h"
#include "Responses.h"
#include "json.hpp"



using json = nlohmann::json;


class JsonRequestPacketDeserializer
{
public:
	static Requests::LoginRequest deserializeLoginRequest(Buffer bufer);
	static Requests::SignupRequest deserializeSignupRequest(Buffer buffer);


};