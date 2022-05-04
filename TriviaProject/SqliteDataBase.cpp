#include "SqliteDataBase.h"

//this is the callback to store the user id on the variable of the class
int SqliteDataBase::callBackUsers(void* userId, int argc, char** argv, char** azColName)
{
	int* user_id = (int*)userId;
	*user_id = std::stoi(argv[0]);
	return 0;
}

//this is the callback function to store the max user id for future purposes, like adding a new user
int SqliteDataBase::callBackMaxUserId(void* maxUserId, int argc, char** argv, char** azColName)
{
	int max = 0;
	int* max_user_id = (int*)maxUserId;
	for (int i = 0; i < argc; i++)
		if (std::string(azColName[i]) == "id")
			if (std::stoi(argv[i]) > max)
				max = std::stoi(argv[i]);
	(*max_user_id) = max + 1;
	return 0;
}

//constructor
SqliteDataBase::SqliteDataBase()
{
	lastUserId = 0;
	maxUserId = 0;
	int res = sqlite3_open(DB_NAME, &db);
	if (res != SQLITE_OK)
		std::cout << "Error opening Data Base" << std::endl;
	std::string sqlStatement = "SELECT * FROM USER_NAME;";
	char** errMessage = { NULL };
	res = sqlite3_exec(db, sqlStatement.c_str(), callBackMaxUserId, &maxUserId, errMessage);
	if (res != SQLITE_OK)
		maxUserId = 1;
}

//destructor
SqliteDataBase::~SqliteDataBase()
{
}

/*
This function checks if the user given exists on the db
input: the username to check
output: true if exsits, false otherwise
*/
bool SqliteDataBase::doesUserExist(const std::string& username)
{
	std::string sqlStatement = "SELECT 1 FROM USER_NAME WHERE user_name='" + username + "';";
	char** errMessage = { NULL };
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, NULL);
	int stat = sqlite3_step(stmt);
	if (stat == SQLITE_DONE)	//if there are no reults
		return false;
	int res = sqlite3_exec(db, sqlStatement.c_str(), callBackUsers, &lastUserId, errMessage);
	if (res != SQLITE_OK)
		return false;
	return true;
}

/*
This function checks if the password given matches to the password of the user given on the db
input: the username and the password
output: true if matches, false otherwise
*/
bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	if(!this->doesUserExist(username))
		return false;
	std::string sqlStatement = "SELECT 1 FROM PASSWORD WHERE user_id=" + std::to_string(lastUserId) + " AND password='" + password + "';";
	char** errMessage = { NULL };
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, NULL);
	int stat = sqlite3_step(stmt);
	if (stat == SQLITE_DONE)	//if there are no results
		return false;
	int res = sqlite3_exec(db, sqlStatement.c_str(), NULL, NULL, errMessage);
	if (res != SQLITE_OK)
		return false;
	return true;
}

/*
This function adds a new user to the data base
input: username, password and email
output: none
*/
void SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	if (!this->doesUserExist(username))
	{
		std::string sqlStatement = "INSERT INTO USER_NAME (user_name, id) VALUES ('" + username + "', " + std::to_string(maxUserId) + ");";
		char** errMessage = { NULL };
		int res = sqlite3_exec(db, sqlStatement.c_str(), NULL, NULL, errMessage);
		if (res != SQLITE_OK)
			std::cout << "Error adding user" << std::endl;

		sqlStatement = "INSERT INTO PASSWORD (password, user_id) VALUES ('" + password + "', " + std::to_string(maxUserId) + ");";
		res = sqlite3_exec(db, sqlStatement.c_str(), NULL, NULL, errMessage);
		if (res != SQLITE_OK)
			std::cout << "Error adding password" << std::endl;


		sqlStatement = "INSERT INTO EMAIL (user_id, email) VALUES (" + std::to_string(maxUserId) + ", '" + email + "');";
		res = sqlite3_exec(db, sqlStatement.c_str(), NULL, NULL, errMessage);
		if (res != SQLITE_OK)
			std::cout << "Error adding email" << std::endl;
		maxUserId++;
	}
}
