#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
	m_handlerFactory = new RequestHandlerFactory;
	m_loginManager = new LoginManager;
}

LoginRequestHandler::~LoginRequestHandler()
{
	delete m_handlerFactory;
	delete m_loginManager;
}

bool LoginRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	return (request.id == SIGNIN_REQUEST || request.id == SIGNUP_REQUEST) ? true : false;
}

Requests::RequestResult LoginRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult result;
	if (this->isRequestRelevant(request))
	{
		result.newHandler = new LoginRequestHandler();
		if (request.id == SIGNIN_REQUEST)
		{

			Requests::LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
			if (loginRequest.username != "")
			{
				if (loginRequest.password != "")
				{
					Responses::LoginResponse response;
					response.status = OK_STATUS;
					result.response = JsonResponsePacketSerializer::serializeResponse(response);
					return result;
				}
			}
			else if (request.id == SIGNUP_REQUEST)
			{
				Requests::SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);

				if (signupRequest.email != "")
				{
					if (signupRequest.username != "")
					{
						if (signupRequest.password != "")
						{
							Responses::LoginResponse response;
							response.status = OK_STATUS;
							result.response = JsonResponsePacketSerializer::serializeResponse(response);
							return result;
						}
					}
				}


			}
		}
		Responses::ErrorResponse errorResponse = { "Error, incorrect state" };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

Requests::RequestResult LoginRequestHandler::signup(Requests::SignupRequest registerDetails)
{
	m_loginManager->signup(registerDetails.email, registerDetails.username, registerDetails.password);
	return Requests::RequestResult();
}
