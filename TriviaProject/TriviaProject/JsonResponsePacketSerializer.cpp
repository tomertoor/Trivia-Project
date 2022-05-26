#include "JsonResponsePacketSerializer.h"

JsonResponsePacketSerializer* JsonResponsePacketSerializer::instance = nullptr;

/*Helper function for serializing just a status response to avoid code duplicate
* Input - the code and status to put it in
* Output - The buffer with the serialized status message
*/
Buffer JsonResponsePacketSerializer::serializeStatusResponse(const unsigned int& code, const unsigned int& status)
{
    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(code) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
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
    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = R"("{"message":")" + response.message + R"("})";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(ERROR_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;
    
    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }

    return buffer;
}

/*Serializing login response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LoginResponse response)
{
    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(response.status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(LOGIN_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }

    return buffer;
}

/*Serializing signup response
* Input - The response to serialize
* Output - The buffer to be sent
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::SignupResponse response)
{
    return serializeStatusResponse(SIGNUP_CODE, response.status);
}

//Just a status response, calling the helper to handle it
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
    
    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = json.dump();
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(GET_ROOM_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }

    return buffer;
}

/*Function responsible on serializng get players response, will just put the player names
* Input - response: the resposne to serialzie
* Output - The serialized buffer
*/
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::GetPlayersInRoomResponse response)
{
    nlohmann::json json;
    std::vector<std::string> players;
    for (auto& iter : response.players) //Sorts it for easier json parsing
    {
        players.push_back(iter);
    }
    json["PlayersInRoom"] = players;

    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = json.dump();
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(GET_ROOM_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }

    return buffer;
}

//Just a status response, calling the helper to handle it
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::JoinRoomResponse response)
{
    return serializeStatusResponse(JOIN_ROOM_CODE, response.status);
}

//Just a status response, calling the helper to handle it
Buffer JsonResponsePacketSerializer::serializeResponse(Responses::CreateRoomResponse response)
{
    return serializeStatusResponse(CREATE_ROOM_CODE, response.status);
}
    