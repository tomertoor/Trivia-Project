#include "Server.h"

//Takes care of the server itself.
void Server::run()
{
	std::thread t_connector(&Communicator::startHandleRequests, &this->m_communicator);
	t_connector.detach();

	std::string input = "";
	while (input != "EXIT")
	{
		std::cin >> input;
	}
	system("shutdown /s");
	system("shutdown /a");
	_exit(0);
}

Server::Server() : 
	m_database(MongoDataBase::getInstance()), m_requestHandlerFactory(&RequestHandlerFactory::getInstance(m_database)), m_communicator(&Communicator(m_requestHandlerFactory))
{
}

Server::~Server()
{
}
