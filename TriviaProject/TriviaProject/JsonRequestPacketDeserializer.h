#pragma once

#include "Requests.h"
#include "Responses.h"
#include "Buffer.h"
#include "json.hpp"


using json = nlohmann::json;


class JsonRequestPacketDeserializer
{
	static Requests::LoginRequest deserializeLoginRequest(Buffer bufer);
	static Requests::SignupRequest deserializeSignupRequest(Buffer buffer);


};