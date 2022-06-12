#pragma once

#include "Responses.h"
#include "Buffer.h"
#include "json.hpp"


using json = nlohmann::json;

#define MAX_FILLING_LENGTH 4

#define SIGNUP_CODE "01"
#define LOGIN_CODE "05"
#define ERROR_CODE "03"
#define LOGOUT_CODE "02"
#define CREATE_ROOM_CODE "04"
#define JOIN_ROOM_CODE "06"
#define GET_ROOM_CODE "07"
#define PERSONAL_STATS_CODE "08"
#define GET_PLAYERS_CODE "09"
#define HIGH_SCORE_CODE "10"
#define CLOSE_ROOM_CODE "11"
#define START_GAME_CODE "12"
#define ROOM_STATE_CODE "13"
#define LEAVE_ROOM_CODE "14"

//Positions for json
#define AVERAGE_ANSWER 3
#define CORRECT_ANSWER 0
#define TOTAL_ANSWERS 1
#define GAME_COUNT 2

class JsonResponsePacketSerializer
{

private:
	static Buffer serializeStatusResponse(const char* code, const unsigned int& status);
	static Buffer serializeJsonResponse(const char* code, const nlohmann::json& json);
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
	static Buffer serializeResponse(Responses::CloseRoomResponse response);
	static Buffer serializeResponse(Responses::StartGameResponse response);
	static Buffer serializeResponse(Responses::GetRoomStateResponse response);
	static Buffer serializeResponse(Responses::LeaveRoomResponse response);



};