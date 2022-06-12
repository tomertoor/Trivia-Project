#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
	m_handlerFactory = RequestHandlerFactory::getInstance();
	m_loginManager = LoginManager::getInstance();
}

LoginRequestHandler::~LoginRequestHandler()
{
}

LoginRequestHandler::LoginRequestHandler(LoginManager* manager, RequestHandlerFactory* factory) : m_handlerFactory(factory), m_loginManager(manager)
{
}

//Checks if the request is a sign in or a signup, if not its false
bool LoginRequestHandler::isRequestRelevant(Requests::RequestInfo request)
{
	if (request.id == std::atoi(LOGIN_CODE) || request.id == std::atoi(SIGNUP_CODE))
	{
		return true;
	}
	return false;
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
		if (request.id == SIGNIN_REQUEST)
		{
			Requests::LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
			return this->login(loginRequest);
		}
		else if (request.id == SIGNUP_REQUEST)
		{
			Requests::SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);
			return this->signup(signupRequest);
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
		LoggedUser user(loginDetails.username);
		Responses::LoginResponse response{ OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = (IRequestHandler*)this->m_handlerFactory->createMenuRequestHandler(user);
	}
	catch (dbException ex)
	{
		Responses::ErrorResponse errorResponse;
		switch (ex.status)
		{
		case WRONG_PASS:
		{
			 errorResponse.message = "Error, wrong password or user doesn't exist.";
			
			break;
		}
		case ALREADY_LOGGED:
		{
			errorResponse.message = "Error, User with this name is already logged in..";
			break;
		}
		default:
		{
			errorResponse.message = "Error, Unexpected behaviour.";
		}
		
		}
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = (IRequestHandler*)this->m_handlerFactory->createLoginRequestHandler();
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
		this->m_loginManager->signup(registerDetails);
		Responses::SignupResponse response{ OK_STATUS };
		result.response = JsonResponsePacketSerializer::serializeResponse(response);

		LoggedUser user(registerDetails.username);

		result.newHandler = (IRequestHandler*)this->m_handlerFactory->createMenuRequestHandler(user);
	}
	catch (dbException ex)
	{
		switch (ex.status)
		{
		case USER_EXIST:
		{
			Responses::ErrorResponse errorResponse{ "Error, user exists." };
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = (IRequestHandler*)this->m_handlerFactory->createLoginRequestHandler();
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
	catch (GlobalException& ge)
	{
		Responses::ErrorResponse errorResponse{ ge.what() };
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = (IRequestHandler*)this->m_handlerFactory->createLoginRequestHandler();
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
