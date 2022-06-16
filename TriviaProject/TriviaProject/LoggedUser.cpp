#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& name)
{
	m_username = name;
}

LoggedUser::LoggedUser()
{
}

LoggedUser::~LoggedUser()
{
}

void LoggedUser::setName(const std::string& username)
{
	m_username = username;
}

std::string LoggedUser::getName() const
{
	return m_username;
}

bool LoggedUser::operator==(const LoggedUser& other)
{
	return other.getName() == this->getName();
}

bool operator==(const LoggedUser& user1, const LoggedUser& user2)
{
	return user1.getName() == user2.getName();
}
