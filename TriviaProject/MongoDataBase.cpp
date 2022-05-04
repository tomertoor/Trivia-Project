#include "MongoDataBase.h"

//constructor
MongoDataBase::MongoDataBase()
{
	uri = mongocxx::uri(URI);
	client = mongocxx::client(uri);
	db = client[DB_NAME];
}

//destructor
MongoDataBase::~MongoDataBase()
{
}

/*
This function checks if the user given exists on the db
input: the username to check
output: true if exsits, false otherwise
*/
bool MongoDataBase::doesUserExist(const std::string& username)
{
	mongocxx::collection coll = db[COLLECTION_NAME];
	mongocxx::cursor cursor = coll.find({});
	for (auto&& doc : cursor)
	{
		bsoncxx::document::element element = doc["username"];
		std::string name = std::string(element.get_utf8().value);
		if (name == username)
			return true;
	}
	return false;
}

/*
This function checks if the password given matches to the password of the user given on the db
input: the username and the password
output: true if matches, false otherwise
*/
bool MongoDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	if (doesUserExist(username))
	{
		mongocxx::collection coll = db[COLLECTION_NAME];
		mongocxx::cursor cursor = coll.find({});
		for (auto&& doc : cursor)
		{
			bsoncxx::document::element element = doc["username"];
			bsoncxx::document::element element2 = doc["password"];
			std::string name = std::string(element.get_utf8().value);
			std::string pass = std::string(element2.get_utf8().value);
			if (name == username && pass == password)
				return true;
		}
	}
	return false;
}

/*
This function adds a new user to the data base
input: username, password and email
output: none
*/
void MongoDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	if (!doesUserExist(username))
	{
		mongocxx::collection coll = db[COLLECTION_NAME];
		auto builder = bsoncxx::builder::stream::document{};
		bsoncxx::document::value newUser = builder 
			<< "username" << username 
			<< "password" << password
			<< "email" << email 
			<< bsoncxx::builder::stream::finalize;
		coll.insert_one(newUser.view());
	}
}
