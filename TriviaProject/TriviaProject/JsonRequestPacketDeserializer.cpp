#include "JsonRequestPacketDeserializer.h"

Requests::LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buffer)
{
	Requests::LoginRequest request;

	std::string str;
	

	std::copy(buffer.buffer.begin() + 6, buffer.buffer.end(), str);
	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	return request;
}

Requests::SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer buffer)
{
	Requests::SignupRequest request;
	
	std::string str;


	std::copy(buffer.buffer.begin() + 6, buffer.buffer.end(), str);
	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	request.email = json["email"];
	return request;
}
