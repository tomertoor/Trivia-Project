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

/*The point calculator function
* Input - the name to calculate for
* Output - the points
*/
double MongoDataBase::calculatePoints(const std::string& name)
{
	try
	{
		double correctAns = (this->getNumOfCorrectAnswers(name));
		double totalAns = (this->getNumOfTotalAnswers(name));
		double gameCount(this->getNumOfPlayerGames(name));
		double result = (correctAns / (2*totalAns)) * 1000 * (gameCount * 0.1);
		return result < 0 ? 0 : result;
	}
	catch (...)
	{
		return 0;
	}
	return 0	;
}

//constructor
MongoDataBase::MongoDataBase() :
	ins()
{
	uri = mongocxx::uri(URI);
	client = mongocxx::client(uri);
	db = client[DB_NAME];
	
	auto test =  this->getHighestScores();
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

/*Returns the average answer time
* Input - name: the name to look for
* Output - the average time
*/
float MongoDataBase::getPlayerAverageAnswerTime(std::string name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name 
		<< finalize
	);
	auto view = row.value().view();
	
	return std::stof(view["averageAnswerTime"].get_decimal128().value.to_string());
}


/*Function that gets the correct amount of answers
* Input - the name to look for
* Output - the number of correct answers
*/
int MongoDataBase::getNumOfCorrectAnswers(std::string name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name
		<< finalize
	);
	auto view = row.value().view();

	return view["correctAnswers"].get_int32().value;
}

/*Function that gets the total amount of answers
* Input - the name to look for
* Output - the number of total answers
*/
int MongoDataBase::getNumOfTotalAnswers(std::string name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name
		<< finalize
	);
	auto view = row.value().view();

	return view["totalAnswers"].get_int32().value;
}

/*Function that gives all the games the player played
* Input - the name to look for
* Output - the num of games
*/
int MongoDataBase::getNumOfPlayerGames(std::string name)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name
		<< finalize
	);
	auto view = row.value().view();

	return view["gameCount"].get_int32().value;
}

/*Function that gets the highest score
* Input - the name to look for
* Output - vector of all of the highest scores
*/
std::vector<std::string> MongoDataBase::getHighestScores()
{
	std::vector<std::pair<std::string, double>> vec;

	for (auto& user : this->db[USER_COLLECTION].find({}))
	{
		auto username = user["username"].get_utf8().value.to_string();
		vec.push_back(std::make_pair(username, this->calculatePoints(username)));
	}
	std::sort(vec.begin(), vec.end(), [](const std::pair<std::string, double>& p1, const std::pair<std::string, double>& p2)
		{
			return p1.second > p2.second;
		});

	std::vector<std::string> finalVec;
	if (vec.size() < TOP_SCORE_AMOUNT)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			finalVec.push_back(vec[i].first);
		}
		return finalVec;
	}
	for (int i = 0; i < TOP_SCORE_AMOUNT; i++)
	{
		finalVec.push_back(vec[i].first);
	}
	return finalVec;
}


