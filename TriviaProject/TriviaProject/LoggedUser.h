#pragma once
#include <string>

class LoggedUser
{
private:
	std::string m_username;
public:
	LoggedUser(const std::string& name);
	LoggedUser();
	~LoggedUser();
	void setName(const std::string& username);
	std::string getName() const;
	bool operator==(const LoggedUser& other);
};