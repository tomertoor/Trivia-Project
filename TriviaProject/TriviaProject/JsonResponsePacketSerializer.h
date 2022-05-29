#pragma once

#include "Responses.h"
#include "Buffer.h"
#include "json.hpp"


using json = nlohmann::json;

#define MAX_FILLING_LENGTH 4

#define SIGNUP_CODE 1
#define LOGIN_CODE 5
#define ERROR_CODE 3
#define LOGOUT_CODE 2
#define CREATE_ROOM_CODE 4
#define JOIN_ROOM_CODE 6
#define GET_ROOM_CODE 7
#define PERSONAL_STATS_CODE 8
#define GET_PLAYERS_CODE 9
#define HIGH_SCORE_CODE 'A'

#define AVERAGE_ANSWER 0
#define CORRECT_ANSWER 1
#define TOTAL_ANSWERS 2
#define GAME_COUNT 3
class JsonResponsePacketSerializer
{

private:
	static Buffer serializeStatusResponse(const unsigned int& code, const unsigned int& status);
	static Buffer serializeJsonResponse(const unsigned int& code, const nlohmann::json& json);
	static Buffer stringToBuffer(std::string str);
	static JsonResponsePacketSerializer* instance;

	JsonResponsePacketSerializer() {};
public:

	static JsonResponsePacketSerializer* getInstance();

	static Buffer serializeResponse(Responses::ErrorResponse response);
	static Buffer serializeResponse(Responses::LoginResponse response);
	static Buffer serializeResponse(Responses::SignupResponse response);
	static Buffer serializeResponse(Responses::LogoutResponse response);
	static Buffer serializeResponse(Responses::GetRoomsResponse response);
	static Buffer serializeResponse(Responses::GetPlayersInRoomResponse response);
	static Buffer serializeResponse(Responses::JoinRoomResponse response);
	static Buffer serializeResponse(Responses::CreateRoomResponse response);

	static Buffer serializeResponse(Responses::GetHighScoreResponse response);
	static Buffer serializeResponse(Responses::GetPersonalStatsResponse response);


};