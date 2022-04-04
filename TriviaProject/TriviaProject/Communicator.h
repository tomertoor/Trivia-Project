#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

class Communicator
{
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
public:
	void startHandleRequests();
	void bindAndListen();
	void handleNewClient(SOCKET);
}