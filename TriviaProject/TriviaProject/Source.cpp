#include "WSAInitializer.h"
#include "Server.h"
#include "OTPCryptoAlgorithm.h"

#pragma warning(disable : 4996)
int main()
{
	WSAInitializer wsa;
	Server* srv = Server::getInstance();
	srv->run();
	return 0;
}
