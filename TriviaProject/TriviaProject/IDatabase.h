#pragma once
#include <string>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email,
		const std::string& phone, const std::string& birthDate,
		const std::string& apt, const std::string& city, const std::string& street) = 0;
};