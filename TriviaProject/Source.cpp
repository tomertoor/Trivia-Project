#include <mongocxx/instance.hpp>
#include "MongoDataBase.h"

int main()
{
	mongocxx::instance ins;
	MongoDataBase mdb;
	mdb.addNewUser("ariel", "12345", "ariel@gmail.com");
	if (mdb.doesPasswordMatch("ariel", "12345"))
		std::cout << "Ariel exists" << std::endl;
	return 0;
}