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
	friend bool operator==(const LoggedUser& user1, const LoggedUser& user2);
};

class UserHash
{
public:
	std::size_t operator()(const LoggedUser& user) const
	{
		std::hash<std::string> hasher;
		return hasher(user.getName());
	}
};