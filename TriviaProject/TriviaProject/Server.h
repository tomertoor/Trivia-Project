#pragma once
#include "Communicator.h"
#include <thread>

class Server
{
private:
	std::shared_ptr<IDatabase> m_database;
	Communicator* m_communicator;
	RequestHandlerFactory* m_requestHandlerFactory;

	Server();

	static Server* instance;

public:

	Server(Server&) = delete;
	Server& operator=(const Server&) = delete;

	~Server();

	void run();
	
	static Server* getInstance();
	
};