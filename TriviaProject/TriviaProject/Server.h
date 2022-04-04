#pragma once
#include "Communicator.h"

class Server
{
private:
	IDatabase* m_database;
	Communicator m_communicator;
	RequestHandlerFactory m_requestHandlerFactory;
public:
	void run();
};