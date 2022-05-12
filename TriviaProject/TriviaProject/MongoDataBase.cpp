#include "MongoDataBase.h"

//return 0 if no user with the name, 1 otherwise
boost::optional<bsoncxx::v_noabi::document::value> MongoDataBase::getUser(std::string username)
{
	return db[USER_COLLECTION].find_one(

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
	return bool(db[USER_COLLECTION].find_one(
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
void MongoDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email,
	const std::string& phone, const std::string& birthDate,
	const std::string& apt, const std::string& city, const std::string& street)
{
	if (!doesUserExist(username))
	{
		mongocxx::collection coll = db[USER_COLLECTION];
		auto builder = bsoncxx::builder::stream::document{};
		bsoncxx::document::value newUser = builder 
			<< "username" << username 
			<< "password" << password
			<< "email" << email 
			<< "phone" << phone
			<< "address" << bsoncxx::builder::stream::open_document
				<< "city" << city
				<< "street" << street
				<< "apt" << apt
				<< bsoncxx::builder::stream::close_document
			<< "birth date" << birthDate
			<< bsoncxx::builder::stream::finalize;
		coll.insert_one(newUser.view());
	}
}
