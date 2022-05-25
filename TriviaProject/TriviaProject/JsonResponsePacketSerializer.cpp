#include "JsonResponsePacketSerializer.h"

JsonResponsePacketSerializer* JsonResponsePacketSerializer::instance = nullptr;

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

/*Serializing error response
* Input - The response to serialize
* Output - The buffer to be sent
*/
JsonResponsePacketSerializer* JsonResponsePacketSerializer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new JsonResponsePacketSerializer();
    }
    return instance;
}
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

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LogoutResponse response)
{
    return serializeStatusResponse(LOGOUT_CODE, response.status);
}

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

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::JoinRoomResponse response)
{
    return serializeStatusResponse(JOIN_ROOM_CODE, response.status);
}

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::CreateRoomResponse response)
{
    return serializeStatusResponse(CREATE_ROOM_CODE, response.status);
}
    