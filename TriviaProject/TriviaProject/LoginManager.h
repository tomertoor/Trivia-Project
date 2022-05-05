#pragma once
#include "MongoDataBase.h"
#include <vector>
#include "LoggedUser.h"
#include "IDatabase.h"
#include "dbException.h"

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
public:
	LoginManager();
	~LoginManager();

	void signup(const std::string& mail, const std::string& username, const std::string& password);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};