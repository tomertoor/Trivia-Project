#pragma once
#include "IDatabase.h"
#include <iostream>
#include <cstdint>
#include <vector>
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

#define DB_NAME "USERS"
#define COLLECTION_NAME "users"
#define URI "mongodb+srv://profzak:nowicanknowhowtousemongodb@cluster0.oyn8b.mongodb.net/myFirstDatabase?retryWrites=true&w=majority"

class MongoDataBase : public IDatabase
{
private:
	mongocxx::uri uri;
	mongocxx::client client;
	mongocxx::database db;
	boost::optional<bsoncxx::v_noabi::document::value> getUser(std::string username);
public:
	MongoDataBase();
	virtual ~MongoDataBase();

	bool doesUserExist(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
};