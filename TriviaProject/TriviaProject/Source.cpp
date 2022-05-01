#include "Server.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer wsa;
	Server server;
	
	server.run();
	return 0;
}