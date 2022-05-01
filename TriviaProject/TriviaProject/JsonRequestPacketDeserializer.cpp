#include "JsonRequestPacketDeserializer.h"

/*Desirializing a buffer to a login request object
* Input - buffer: the buffer to desirialize
* Output - the request desirialized
*/
Requests::LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buffer)
{
	Requests::LoginRequest request;

	std::string str = "";
	
	for (auto it = buffer.buffer.begin(); it != buffer.buffer.end(); it++)
	{
		str += *it;
	}

	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	return request;
}

/*Desirializing a buffer to a signup request object
* Input - buffer: the buffer to desirialize
* Output - the request desirialized
*/
Requests::SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer buffer)
{
	Requests::SignupRequest request;
	
	std::string str = "";

	for (auto it = buffer.buffer.begin(); it != buffer.buffer.end(); it++)
	{
		str += *it;
	}


	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	request.email = json["email"];
	return request;
}
