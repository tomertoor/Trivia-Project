#pragma once
#include "IDatabase.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <set>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>		
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

#define TOP_SCORE_AMOUNT 5

#define DB_NAME "USERS"
#define USER_COLLECTION "users"
#define QUESTION_COLLECTION "questions"
#define STATS_COLLECTION "statistics"
#define URI "mongodb+srv://tomertoor12:wYsZDmvUo^7*Vb@cluster0.g4kew.mongodb.net/myFirstDatabase?retryWrites=true&w=majority"

class MongoDataBase : public IDatabase
{
private:
	mongocxx::instance ins;
	mongocxx::uri uri;
	mongocxx::client client;
	mongocxx::database db;
	boost::optional<bsoncxx::v_noabi::document::value> getUser(std::string username);

	double calculatePoints(const std::string& username);
public:
	MongoDataBase();
	virtual ~MongoDataBase() = default;

	MongoDataBase(const MongoDataBase&) = delete;
	MongoDataBase& operator=(const MongoDataBase&) = delete;

	//get instance for singleton
	static std::shared_ptr<IDatabase> getInstance()
	{
		static std::shared_ptr<IDatabase> instance(new MongoDataBase());

		return instance;
	}


	bool doesUserExist(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& email,
		const std::string& phone, const std::string& birthDate,
		const std::string& apt, const std::string& city, const std::string& street) override;

	float getPlayerAverageAnswerTime(std::string name) override;
	int getNumOfCorrectAnswers(std::string name) override;
	int getNumOfTotalAnswers(std::string name) override;
	int getNumOfPlayerGames(std::string name) override;

	std::vector<std::string> getHighestScores() override;

	

	void addQuestion(const std::string& name);


};