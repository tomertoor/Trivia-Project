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
	static std::string vectorToString(std::vector<unsigned char> vec);
public:

	static JsonRequestPacketDeserializer* getInstance();

	static Requests::LoginRequest deserializeLoginRequest(Buffer bufer);
	static Requests::SignupRequest deserializeSignupRequest(Buffer buffer);
	static Requests::GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(Buffer buffer);
	static Requests::JoinRoomRequest deserializeJoinRoomRequest(Buffer buffer);
	static Requests::CreateRoomRequest deserializeCreateRoomRequest(Buffer buffer);

};