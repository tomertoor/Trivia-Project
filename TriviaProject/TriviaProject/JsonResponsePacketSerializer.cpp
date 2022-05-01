#include "JsonResponsePacketSerializer.h"

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

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::SignupResponse response)
{
    Buffer buffer;
    std::stringstream message;
    std::string jsonMessage = R"("{"status":")" + std::to_string(response.status) + R"("}")";
    std::string length = std::to_string(jsonMessage.length());
    message << std::to_string(SIGNUP_CODE) << std::setfill('0') << std::setw(MAX_FILLING_LENGTH) << length << jsonMessage;

    std::string messageStr = message.str();

    for (auto it = messageStr.begin(); it != messageStr.end(); it++)
    {
        buffer.buffer.push_back(*it);
    }

    return buffer;
}
