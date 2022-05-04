#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>

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
	void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
};