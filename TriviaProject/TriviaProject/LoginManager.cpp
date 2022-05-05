#include "LoginManager.h"
#include "MongoDataBase.h"
#include <mongocxx/instance.hpp>


LoginManager::LoginManager()
{
	m_database = new MongoDataBase;
}

LoginManager::~LoginManager()
{
}

/*
This function signes up a new user, if not exists on the data base
input: user name, password and email
output: none
*/
void LoginManager::signup(const std::string& mail, const std::string& username, const std::string& password)
{
	if (!m_database->doesUserExist(username))
	{
		m_database->addNewUser(username, password, mail);
		LoggedUser lg(username);
		m_loggedUsers.push_back(lg);
	}
	else
	{
		throw dbException(USER_EXIST);
	}
}

/*
This function logges in an exisiting user, if already registered
input: password and user name
output: none
*/
void LoginManager::login(const std::string& username, const std::string& password)
{
	if (m_database->doesPasswordMatch(username, password))
	{
		LoggedUser lg(username);
		m_loggedUsers.push_back(lg);
	}
	else
	{
		throw dbException(WRONG_PASS);
	}
}

/*
This fucntion logges out a user, if he is logged in
input: username
output: none
*/
void LoginManager::logout(const std::string& username)
{
	for (auto it = m_loggedUsers.begin() ; it != m_loggedUsers.end() ; it++) 
	{
		if (it->getName() == username)
			m_loggedUsers.erase(it);
	}
}
