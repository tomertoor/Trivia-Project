#include "MongoDataBase.h"

MongoDataBase* MongoDataBase::instance = nullptr;

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


//helper function to test questions by adding to db, uses question api
void MongoDataBase::testQuestions()
{
	auto res = cpr::Get(cpr::Url("https://opentdb.com/api.php?amount=10"));
	nlohmann::json json = nlohmann::json::parse(res.text);
	for (auto& iter : json["results"])
	{
		srand(time(NULL));
		int correctIndex = rand() % 4, i = 0;
		std::vector<std::string> answers;
		auto it = iter["incorrect_answers"].begin();
		for (int i = 0 ; i < 4; i++)
		{
			if (correctIndex == i)
				answers.push_back(iter["correct_answer"]);
			else
			{
				answers.push_back(*it);
				it++;
			}
		}
		Question question(iter["question"], answers, correctIndex);
		this->addQuestion(question);
	}
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
		mongocxx::collection userColl = db[USER_COLLECTION];
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
		userColl.insert_one(newUser.view());
		
		mongocxx::collection statsColl = db[STATS_COLLECTION];
		auto statsBuilder = bsoncxx::builder::stream::document{};
		bsoncxx::document::value newStats = statsBuilder
			<< "name" << username
			<< "gameCount" << 0
			<< "totalAnswers" << 0
			<< "correctAnswers" << 0
			<< "averageAnswerTime" << 0.0
			<< bsoncxx::builder::stream::finalize;
		statsColl.insert_one(newStats.view());
	}
}

/*Returns the average answer time
* Input - name: the name to look for
* Output - the average time
*/
double MongoDataBase::getPlayerAverageAnswerTime(std::string name) 
{
	std::lock_guard<std::mutex> guard(dbMutex);
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name 
		<< finalize
	);
	auto view = row.value().view();
	return view["averageAnswerTime"].get_double().value;
}


/*Function that gets the correct amount of answers
* Input - the name to look for
* Output - the number of correct answers
*/
int MongoDataBase::getNumOfCorrectAnswers(std::string name) 
{
	std::lock_guard<std::mutex> guard(dbMutex);
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
	std::lock_guard<std::mutex> guard(dbMutex);
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
	std::lock_guard<std::mutex> guard(dbMutex);
	bsoncxx::stdx::optional<bsoncxx::document::value> row = db[STATS_COLLECTION].find_one(
		document{}
		<< "name" << name
		<< finalize
	);
	auto view = row.value().view();

	return view["gameCount"].get_int32().value;
}

/*Function that update statistics
* Input - the name to update for and the results to append to the db
* Output - None.
*/
void MongoDataBase::updateStatistics(const std::string& name, GameData newResults)
{
	int correctAns = this->getNumOfCorrectAnswers(name);
	PlayerResults overallResults{ name, correctAns, this->getNumOfTotalAnswers(name) - correctAns, this->getPlayerAverageAnswerTime(name) };
	overallResults.correctAnswerCount += newResults.correctAnswerCount;
	overallResults.wrongAnswerCount += newResults.wrongAnswerCount;
	overallResults.averageAnswerTime = ( ((int)overallResults.correctAnswerCount + overallResults.wrongAnswerCount) * overallResults.averageAnswerTime + newResults.averageAnswerTime) / ((int)overallResults.correctAnswerCount + overallResults.wrongAnswerCount + 1);

	mongocxx::collection coll = db[STATS_COLLECTION];
	// switching from struct to regular vars since mongo likes to make errors if not
	int totalAnswers = overallResults.wrongAnswerCount + overallResults.correctAnswerCount;
	int correctAnswers = overallResults.correctAnswerCount;
	double averageAnswerTime = overallResults.averageAnswerTime;

	std::lock_guard<std::mutex> guard(dbMutex);
	db[STATS_COLLECTION].update_one(document{} << "name" << name << finalize, document{} 
			<< "$inc" << open_document <<"gameCount" << 1 << close_document 
			<< "$set" << open_document
			<< "totalAnswers" << totalAnswers
			<< "averageAnswerTime" << averageAnswerTime
			<< "correctAnswers" << correctAnswers << close_document << finalize);


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
/*Adds a question
* Input - the question to add
* Output - none.
*/
void MongoDataBase::addQuestion(Question& question)
{
	mongocxx::collection coll = db[QUESTION_COLLECTION];

	bsoncxx::builder::stream::array bsonArr;

	for (auto& iter : question.getPossibleAnswers())
	{
		bsonArr << iter;
	}
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value newQuestion = builder
		<< "content" << question.getQuestion()
		<< "correctAnswerId" << question.getCorrectAnswerId()
		<< "possibleAnswers" << bsonArr 
		<< bsoncxx::builder::stream::finalize;
	coll.insert_one(newQuestion.view());
}

/*Returns x amount of questions from the db
* Input - the amount of questions to get
* Output - The list of questions
*/
std::list<Question> MongoDataBase::getQuestions(int amount)
{
	mongocxx::collection coll = db[QUESTION_COLLECTION];
	std::list<Question> questions;
	int count = 0;
	mongocxx::pipeline pipe{};
	pipe.sample(int32_t(amount));
	mongocxx::cursor cursor = coll.aggregate(pipe, mongocxx::options::aggregate{});
	for (auto& doc : cursor)
	{
		if (count == amount)
		{
			break;
		}
		std::string content = doc["content"].get_utf8().value.to_string();
		int correctAnswerId = doc["correctAnswerId"].get_int32();
		std::vector<std::string> answers;
		for (auto& ans : doc["possibleAnswers"].get_array().value)
			answers.push_back(ans.get_utf8().value.to_string());
		Question question(content, answers, correctAnswerId);
		questions.push_back(question);
		count++;
	}
	return questions;
}


