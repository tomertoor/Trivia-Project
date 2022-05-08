#include "MongoDataBase.h"

//return 0 if no user with the name, 1 otherwise
boost::optional<bsoncxx::v_noabi::document::value> MongoDataBase::getUser(std::string username)
{
	return db[COLLECTION_NAME].find_one(

		document{}
		<< "username" << username
		<< finalize
	);
}

//constructor
MongoDataBase::MongoDataBase() :
	ins()
{
	uri = mongocxx::uri(URI);
	client = mongocxx::client(uri);
	db = client[DB_NAME];
}

std::shared_ptr<IDatabase> MongoDataBase::getInstance()
{
	static std::shared_ptr<IDatabase> instance(new MongoDataBase());

	return instance;
}

/*
This function checks if the user given exists on the db
input: the username to check
output: true if exsits, false otherwise
*/
bool MongoDataBase::doesUserExist(const std::string& username)
{
	return bool(getUser(username));
}

/*
This function checks if the password given matches to the password of the user given on the db
input: the username and the password
output: true if matches, false otherwise
*/
bool MongoDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	return bool(db[COLLECTION_NAME].find_one(
		document{}
		<< "username" << username
		<< "password" << password
		<< finalize
	));
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
