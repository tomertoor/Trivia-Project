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
	void setName();
	std::string getName() const;
};