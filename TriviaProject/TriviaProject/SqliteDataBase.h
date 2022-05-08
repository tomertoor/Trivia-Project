#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <tuple>

#define DB_NAME "users.sqlite"

class SqliteDataBase : public IDatabase
{
private:
	sqlite3* db;
	int lastUserId;
	int maxUserId;
	static int callBackUsers(void* userId, int argc, char** argv, char** azColName);
	static int callBackMaxUserId(void* maxUserId, int argc, char** argv, char** azColName);
public:
	SqliteDataBase();
	virtual ~SqliteDataBase();

	bool doesUserExist(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& email,
		const std::string& phone, const std::string& birthDate,
		const std::string& apt, const std::string& city, const std::string& street) override;
};