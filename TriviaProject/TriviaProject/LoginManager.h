#pragma once
#include <vector>
#include "LoggedUser.h"
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "dbException.h"

class LoginManager
{
private:
	std::shared_ptr<IDatabase> m_database;
	std::vector<LoggedUser> m_loggedUsers;
	LoginManager(std::shared_ptr<IDatabase>& db);
	~LoginManager() = default;
public:
	LoginManager(const LoginManager&) = delete;
	LoginManager& operator=(const LoginManager&) = delete;

	static LoginManager& getInstance(std::shared_ptr<IDatabase>& database);

	void signup(const std::string& mail, const std::string& username, const std::string& password);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};