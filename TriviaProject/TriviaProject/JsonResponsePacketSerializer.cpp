#include "JsonResponsePacketSerializer.h"

JsonResponsePacketSerializer* JsonResponsePacketSerializer::instance = nullptr;

/*Helper function to avoid code duplication, serializes based on json of status only
* Input - code to put and json
* Output - the serilized buffer.
*/
Buffer JsonResponsePacketSerializer::serializeStatusResponse(const unsigned int& code, const unsigned int& status)
{
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(code) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
}

/*Helper function to avoid code duplication, serializes based on json
* Input - code to put and json
* Output - the serilized buffer.
*/
Buffer JsonResponsePacketSerializer::serializeJsonResponse(const unsigned int& code, const nlohmann::json& json)
{
    std::stringstream message;
    std::string jsonMessage = json.dump();
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(code) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

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
    message << std::to_string(ERROR_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
}

/*Serializing login response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LoginResponse response)
{
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(response.status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(LOGIN_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    return stringToBuffer(message.str());
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
    for (auto& iter : response.rooms) //Sorts it for easier json parsing
    {
        names.push_back(iter.name);
    }
    json["rooms"] = names;

    return serializeJsonResponse(GET_ROOM_CODE, json);
}

/*Serializes GetPlayersInRoom response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetPlayersInRoomResponse response)
{
    nlohmann::json json;
    json["PlayersInRoom"] = response.players;
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
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetHighScoreResponse response)
{
    nlohmann::json json;
    json["UserStatistics"]["averageAnswerTime"] = response.statistics[AVERAGE_ANSWER];
    json["UserStatistics"]["correctAnswers"] = response.statistics[CORRECT_ANSWER];
    json["UserStatistics"]["totalAnswers"] = response.statistics[TOTAL_ANSWERS];
    json["UserStatistics"]["gameCount"] = response.statistics[GAME_COUNT];


    return serializeJsonResponse(HIGH_SCORE_CODE, json);
}

/*Serializes statistics response, json type serialize
* Input - the response to serialize
* Output - the buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetPersonalStatsResponse response)
{
    nlohmann::json json;


    json["HighScores"] = response.statistics;

    return serializeJsonResponse(PERSONAL_STATS_CODE, json);


}
