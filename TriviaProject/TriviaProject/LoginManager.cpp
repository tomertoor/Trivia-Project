#include "LoginManager.h"
#include "MongoDataBase.h"
#include <regex>
#include "GlobalException.h"
#include "Address.h"

LoginManager* LoginManager::instance = nullptr;

LoginManager::LoginManager()
{
	m_database = MongoDataBase::getInstance();
}

//singleton getinstance
LoginManager* LoginManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new LoginManager();
	}
	return instance;
}

LoginManager::~LoginManager()
{
}

/*
This function signes up a new user, if not exists on the data base
input: user name, password and email
output: none
*/
void LoginManager::signup(Requests::SignupRequest signupReq)
{
	if (!m_database->doesUserExist(signupReq.username))
	{
		if (isValidDetails(signupReq))
		{
			m_database->addNewUser(signupReq.username, signupReq.password, signupReq.email, signupReq.phone,
				signupReq.birthDate, signupReq.address.apt, signupReq.address.city, signupReq.address.street);
			m_loggedUsers.push_back(signupReq.username);
		}
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
		if (this->isAlreadyLogged(LoggedUser(username)))
		{
			throw dbException(ALREADY_LOGGED);
		}
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
	for (unsigned int i = 0; i < this->m_loggedUsers.size(); i++) 
	{
		if (this->m_loggedUsers[i].getName() == username)
			m_loggedUsers.erase(this->m_loggedUsers.begin() + i); //erases the cell
	}
}


//all these functions helps to check if the signup details are valid
bool LoginManager::isPasswordValid(const std::string& password)
{
	std::regex passRegex(R"((?=.*\d.*)(?=.*[a-zA-Z].*)(?=.*[\!\@\#\$\%\^\&\*].*).{8,})");
	return regex_match(password, passRegex);
}

bool LoginManager::isMailValid(const std::string& email)
{
	std::regex mailRegex(R"([a-zA-Z0-9]+@[a-zA-Z0-9]+[.][a-zA-Z0-9]+([.][a-zA-Z0-9]+)?)");
	return regex_match(email, mailRegex);
}

bool LoginManager::isAdressValid(const Address& adress)
{
	std::regex onlyLettersRegex(R"([a-zA-Z]+)");
	std::regex onlyDigitRegex(R"([0-9]+)");

	return	regex_match(adress.street, onlyLettersRegex) &&
		regex_match(adress.apt, onlyDigitRegex) &&
		regex_match(adress.city, onlyLettersRegex);
}

bool LoginManager::isPhoneValid(const std::string& phone)
{
	std::regex phoneRegex(R"(^0[\d]{1,2}[-][\d]{7,9}$)");
	return regex_match(phone, phoneRegex);
}

/*Function to check if user is already logged, not using find just in case iterators decide to glitch the vector like in previous versions
* Input - user: the user to check for
* Output - if it's already logged in or not.
*/
bool LoginManager::isAlreadyLogged(LoggedUser user)
{
	for (unsigned int i = 0; i < this->m_loggedUsers.size(); i++)
	{
		if (this->m_loggedUsers[i].getName() == user.getName())
		{
			return true;
		}
	}
	return false;
}

bool LoginManager::isValidDetails(Requests::SignupRequest signupReq)
{
	std::string errMsg = "";
	if (!isPasswordValid(signupReq.password))
		errMsg = "Your password is not following the rules: 8+ chars, 1+ number, uppercase, lowercase and unique";
	else if (signupReq.username == "")
		errMsg = "Can't be empty username";
	else if (!isMailValid(signupReq.email))
		errMsg = "Your email must be in regular email format";
	else if (!isAdressValid(signupReq.address))
		errMsg = "Your address is not following the rules: street & city must include no digit, apt must include digit only";
	else if (!isPhoneValid(signupReq.phone))
		errMsg = "Your phone is not following the rules: start with 0, include '-' after first 2/3 digits, 9-10 digits in total";
	else if (!isDateValid(signupReq.birthDate))
		errMsg = "Your birth date is not following the rules: <DD>.<MM>.<YYYY> OR <DD>/<MM>/<YYYY>";

	if (errMsg == "")
		return true;
	throw GlobalException(errMsg);
}

bool LoginManager::isDateValid(const std::string& date)
{
	std::regex dateRegex(R"(^[\d]{2}[.|/][\d]{2}[.|/][\d]{4}$)");
	return regex_match(date, dateRegex);
}
