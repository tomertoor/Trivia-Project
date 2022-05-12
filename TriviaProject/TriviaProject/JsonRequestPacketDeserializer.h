#pragma once

#include "IRequestHandler.h"
#include "Responses.h"
#include "json.hpp"



using json = nlohmann::json;


class JsonRequestPacketDeserializer
{

private:
	JsonRequestPacketDeserializer() {};
	
	static JsonRequestPacketDeserializer* instance;

public:

	static JsonRequestPacketDeserializer* getInstance();

	static Requests::LoginRequest deserializeLoginRequest(Buffer bufer);
	static Requests::SignupRequest deserializeSignupRequest(Buffer buffer);


};