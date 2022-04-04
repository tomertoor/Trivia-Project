#pragma once
#include "sqlite3.h"

class IDatabase
{
protected:
	sqlite3* m_database;
public:
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual void addNewUser(const std::string& username, const std::string& password) = 0;
};