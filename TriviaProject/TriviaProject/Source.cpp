#include "WSAInitializer.h"
#include "Server.h"

int main()
{
	WSAInitializer wsa;
	Server srv;

	srv.run();
	return 0;
}
