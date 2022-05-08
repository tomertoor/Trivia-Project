#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(LoginManager* manager, RequestHandlerFactory* factory) : m_handlerFactory(factory), m_loginManager(manager)
{
}

//Checks if the request is a sign in or a signup, if not its false
bool LoginRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	return (request.id == SIGNIN_REQUEST || request.id == SIGNUP_REQUEST) ? true : false;
}

/*Handles the sign in and sign up requests
* Input - request: the request info
* Output - The result of the request
*/
Requests::RequestResult LoginRequestHandler::handleRequest(Requests::RequestInfo request)
{
	Requests::RequestResult result;

	if (this->isRequestRelevant(request))
	{
		//result.newHandler = new LoginRequestHandler();
		if (request.id == SIGNIN_REQUEST)
		{

			Requests::LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
			if (loginRequest.username != "")
			{
				if (loginRequest.password != "")
				{
					/*Responses::LoginResponse response;
					response.status = OK_STATUS;
					result.response = JsonResponsePacketSerializer::serializeResponse(response);*/
					return this->login(loginRequest);
				}
			}

		}
		else if (request.id == SIGNUP_REQUEST)
		{
			Requests::SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);

			if (this->isEmailValid(signupRequest.email))
			{
				if (this->isValidUserName(signupRequest.username))
				{
					if (signupRequest.password != "")
					{
						/*Responses::SignupResponse response;
						response.status = OK_STATUS;
						result.response = JsonResponsePacketSerializer::serializeResponse(response);*/
						return this->signup(signupRequest);
					}
				}
			}


		}
		Responses::ErrorResponse errorResponse = { "Error, incorrect state" };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		return result;
	}
}

/*Handles the login request, gets the details and uses the loginmanager, if successful will return a result with menu handler and the buffer, if not it will catch exception with the problem.
* Input - loginDetails: the login details
* Output - The request result
*/
Requests::RequestResult LoginRequestHandler::login(Requests::LoginRequest loginDetails)
{
	Requests::RequestResult result;
	try
	{
		this->m_loginManager->login(loginDetails.username, loginDetails.password);
		Responses::LoginResponse response{ OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler();
	}
	catch (dbException ex)
	{
		switch (ex.status)
		{
		case WRONG_PASS:
		{
			Responses::ErrorResponse errorResponse{ "Error, wrong password or user doesn't exist." };
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = this->m_handlerFactory->createLoginRequestHandler();
			break;
		}
		default:
		{
			Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = nullptr;
		}
		}
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
	return result;
}

/*Handles the signup request, gets the details and uses the loginmanager, if successful will return a result with menu handler and the buffer, if not it will catch exception with the problem.
* Input - registerDetails: the signup details
* Output - The request result
*/
Requests::RequestResult LoginRequestHandler::signup(Requests::SignupRequest registerDetails)
{
	Requests::RequestResult result;
	try
	{
		this->m_loginManager->signup(registerDetails.email, registerDetails.username, registerDetails.password);
		Responses::SignupResponse response{ OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_handlerFactory->createMenuRequestHandler();
	}
	catch (dbException ex)
	{
		switch (ex.status)
		{
		case USER_EXIST:
		{
			Responses::ErrorResponse errorResponse{ "Error, user exists." };
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = this->m_handlerFactory->createLoginRequestHandler();
			break;
		}

		default:
		{
			Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = nullptr;
		}

		}
	}
	catch (...)
	{
		Responses::ErrorResponse errorResponse{ "Error, Unexpected behaviour." };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = nullptr;
	}
	return result;
}

/*Checks if the username is valid, checks length between min and max, checks that first letter is alpha and that its not empty, after that will check on all the other letters
* Input - username: the username to check
* Output - if its valid or not
*/
bool LoginRequestHandler::isValidUserName(const std::string& username)
{
	if (username.length() < MIN_NAME_LENGTH || username.length() > MAX_NAME_LENGTH)
	{
		return false;
	}

	if (username.empty() || !std::isalpha(username.front()))
	{
		return false;
	}

	/*if (std::find_if(username.begin(), username.end(), std::isalpha) != username.end())
	{
		return false;

	}*/
	return true;
}

/*Uses regex as explained below to check if email is actually valid
* Input - email: the email to check
* Output - if its valid or not
*/
bool LoginRequestHandler::isEmailValid(const std::string& email)
{
	std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"); //first of all checks that first char is a letter, then continues to get chars, makes sure there is a @ before the . and then checks that there is text between and finally a dot  and text after it
	return std::regex_match(email, pattern);
}
