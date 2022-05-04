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
	_exit(0);
}

Server::Server() : m_communicator(this->m_requestHandlerFactory)
{
}

Server::~Server()
{
}
