#include "JsonRequestPacketDeserializer.h"

JsonRequestPacketDeserializer* JsonRequestPacketDeserializer::instance = nullptr;

/*Helper function to convert vector to string
* Input - vec: the unsigned char vector
* Output - the vector converted to string
*/

std::string JsonRequestPacketDeserializer::vectorToString(std::vector<unsigned char> vec)
{

	std::string str = "";

	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		str += *it;
	}
	return str;
}

JsonRequestPacketDeserializer* JsonRequestPacketDeserializer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new JsonRequestPacketDeserializer();
	}
	return instance;
}
Requests::LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer buffer)
{
	Requests::LoginRequest request;

	std::string str = vectorToString(buffer.buffer);
	

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
	
	std::string str = vectorToString(buffer.buffer);

	json json = json::parse(str);
	request.username = json["username"];
	request.password = json["password"];
	request.email = json["email"];
	request.phone = json["phone"];
	request.birthDate = json["birthDate"];
	request.address.apt = json["apt"];
	request.address.city = json["city"];
	request.address.street = json["street"];
	return request;
}

/*Desirializing a buffer to a GetPlayersInRoomRequest request object
* Input - buffer: the buffer to desirialize
* Output - the request desirialized
*/
Requests::GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(Buffer buffer)
{
	Requests::GetPlayersInRoomRequest request;

	std::string str = vectorToString(buffer.buffer);

	json json = json::parse(str);
	request.roomId = json["roomId"];

	return request;
}

/*Desirializing a buffer to a JoinRoomRequest request object
* Input - buffer: the buffer to desirialize
* Output - the request desirialized
*/
Requests::JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer buffer)
{
	Requests::JoinRoomRequest request;

	std::string str = vectorToString(buffer.buffer);

	json json = json::parse(str);
	request.roomId = json["roomId"];

	return request;
}

/*Desirializing a buffer to a CreateRoomRequest request object
* Input - buffer: the buffer to desirialize
* Output - the request desirialized
*/
Requests::CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer buffer)
{
	Requests::CreateRoomRequest request;
	std::string str = vectorToString(buffer.buffer);

	json json = json::parse(str);

	request.maxUsers = json["maxUsers"];
	request.questionCount = json["questionCount"];
	request.questionTimeout = json["questionTimeout"];
	request.roomName = json["roomName"];

	return request;
}
