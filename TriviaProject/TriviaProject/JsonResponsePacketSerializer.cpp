#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::ErrorResponse response)
{
	return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::LoginResponse response)
{
	return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(Responses::SignupResponse response)
{
	return Buffer();
}
