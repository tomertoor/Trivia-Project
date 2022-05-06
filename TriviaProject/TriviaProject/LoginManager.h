#pragma once
#include <vector>
#include "LoggedUser.h"
#include "IDatabase.h"
#include "SqliteDataBase.h"

class LoginManager
{
private:
	std::shared_ptr<IDatabase> m_database;
	std::vector<LoggedUser> m_loggedUsers;
public:
	LoginManager();
	~LoginManager();

	void signup(const std::string& mail, const std::string& username, const std::string& password);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};