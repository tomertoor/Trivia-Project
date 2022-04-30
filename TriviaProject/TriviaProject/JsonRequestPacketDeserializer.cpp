#include "JsonRequestPacketDeserializer.h"

Requests::LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buffer)
{
	Requests::LoginRequest request;

	std::string str = "";
	
	for (auto it = buffer.buffer.begin() + 5; it != buffer.buffer.end(); it++)
	{
		str += *it;
	}

	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	return request;
}

Requests::SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer buffer)
{
	Requests::SignupRequest request;
	
	std::string str = "";

	for (auto it = buffer.buffer.begin() + 5; it != buffer.buffer.end(); it++)
	{
		str += *it;
	}

	std::cout << str << std::endl;

	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	request.email = json["email"];
	return request;
}
