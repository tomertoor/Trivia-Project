#include "JsonResponsePacketSerializer.h"

JsonResponsePacketSerializer* JsonResponsePacketSerializer::instance = nullptr;

/*Helper function to avoid code duplication, serializes based on json of status only
* Input - code to put and json
* Output - the serilized buffer.
*/
Buffer JsonResponsePacketSerializer::serializeStatusResponse(const char* code, const unsigned int& status)
{
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::string(code) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
}

/*Helper function to avoid code duplication, serializes based on json
* Input - code to put and json
* Output - the serilized buffer.
*/
Buffer JsonResponsePacketSerializer::serializeJsonResponse(const char* code, const nlohmann::json& json)
{
    std::stringstream message;
    std::string jsonMessage = json.dump();
    std::string length = std::to_string(jsonMessage.length());
    message << std::string(code) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
}

/*Helper function to turn string to buffer
* Input - the str to turn to buffer
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::stringToBuffer(std::string str)
{
    Buffer buffer;
    for (auto it = str.begin(); it != str.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }
    return buffer;
}

//Function for singleton
JsonResponsePacketSerializer* JsonResponsePacketSerializer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new JsonResponsePacketSerializer();
    }
    return instance;
}

/*Serializing error response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::ErrorResponse response)
{
    std::stringstream message;
    std::string jsonMessage = R"("{"message":")" + response.message + R"("})";
    std::string length = std::to_string(jsonMessage.length());
    message << ERROR_CODE << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
}

/*Serializing login response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LoginResponse response)
{
    return serializeStatusResponse(LOGIN_CODE, response.status);
}

/*Serializing signup response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::SignupResponse response)
{
    return serializeStatusResponse(SIGNUP_CODE, response.status);
}

/*Serializes LogoutResponse response, status type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LogoutResponse response)
{
    return serializeStatusResponse(LOGOUT_CODE, response.status);
}

/*Function serializing getroomsresponse, converts room to just room names into json.
* Input - the response
* Output - the serialized message
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetRoomsResponse response)
{
    nlohmann::json json;
    std::vector<std::string> names;
    json["status"] = response.status;
    for (auto& iter : response.rooms) //Sorts it for easier json parsing
    {
        names.push_back(iter.name);
    }
    json["rooms"] = names;

    return serializeJsonResponse(GET_ROOM_CODE, json);
}

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LeaveRoomResponse response)
{
    return serializeStatusResponse(LEAVE_ROOM_CODE, response.status);
}

/*Serializes GetPlayersInRoom response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetPlayersInRoomResponse response)
{
    nlohmann::json json;
    json["playersInRoom"] = response.players;
    return serializeJsonResponse(GET_PLAYERS_CODE, json);
}

/*Serializes JoinRoom response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::JoinRoomResponse response)
{
    return serializeStatusResponse(JOIN_ROOM_CODE, response.status);
}

/*Serializes createRoom response
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::CreateRoomResponse response)
{
    return serializeStatusResponse(CREATE_ROOM_CODE, response.status);
}

/*Serializes highscore response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetPersonalStatsResponse response)
{
    nlohmann::json json;
    json["status"] = response.status;
    json["averageAnswerTime"] = response.statistics[AVERAGE_ANSWER];
    json["correctAnswers"] = response.statistics[CORRECT_ANSWER];
    json["totalAnswers"] = response.statistics[TOTAL_ANSWERS];
    json["gameCount"] = response.statistics[GAME_COUNT];


    return serializeJsonResponse(PERSONAL_STATS_CODE, json);
}

/*Serializes statistics response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetHighScoreResponse response)
{
    nlohmann::json json;

    json["status"] = response.status;
    json["topScores"] = response.statistics;

    return serializeJsonResponse(HIGH_SCORE_CODE, json);


}

/*Serializes closeRoom response, status type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::CloseRoomResponse response)
{
    return serializeStatusResponse(CLOSE_ROOM_CODE, response.status);
}

/*Serializes startgame response, status type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::StartGameResponse response)
{
    return serializeStatusResponse(START_GAME_CODE, response.status);
}

/*Serializes getroom response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetRoomStateResponse response)
{
    nlohmann::json json;
    json["status"] = response.status;
    json["hasGameBegun"] = response.hasGameBegun;
    json["players"] = response.players;
    json["questionCount"] = response.questionCount;
    json["answerTimeout"] = response.answerTimeout;
    
    return serializeJsonResponse(ROOM_STATE_CODE, json);
}
