#pragma once
#include <vector>
#include "LoggedUser.h"
#include "IDatabase.h"
#include "dbException.h"
#include "IRequestHandler.h"

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	bool isValidDetails(Requests::SignupRequest signupReq);
	bool isDateValid(const std::string& date);
	bool isPasswordValid(const std::string& password);
	bool isMailValid(const std::string& email);
	bool isAdressValid(const Address& adress);
	bool isPhoneValid(const std::string& phone);

	bool isAlreadyLogged(LoggedUser user);

	LoginManager();

	static LoginManager* instance;

public:

	LoginManager(LoginManager&) = delete;
	LoginManager& operator=(const LoginManager&) = delete;

	static LoginManager* getInstance();

	~LoginManager();

	void signup(Requests::SignupRequest signupReq);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};