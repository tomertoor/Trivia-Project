#include "Server.h"

Server* Server::instance = nullptr;

//Takes care of the server itself.
void Server::run()
{
	std::thread t_connector(&Communicator::startHandleRequests, this->m_communicator);
	t_connector.detach();

	std::string input = "";
	while (input != "EXIT")
	{
		std::cin >> input;
	}
	_exit(0);
}

//singelton returns the instance
Server* Server::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Server();
	}
	return instance;
}

Server::Server()
{
	this->m_requestHandlerFactory = RequestHandlerFactory::getInstance();
	this->m_communicator = Communicator::getInstance(*this->m_requestHandlerFactory);
}

Server::~Server()
{
}
