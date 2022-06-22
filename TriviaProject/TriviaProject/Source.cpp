#include "WSAInitializer.h"
#include "Server.h"
#include "OTPCryptoAlgorithm.h"

#pragma warning(disable : 4996)
int main()
{
	WSAInitializer wsa;
	Server* srv = Server::getInstance();
	OTPCryptoAlgorithm algorithm;
	std::string before = "checking..";
	auto after = algorithm.Encrypt(before);
	before = algorithm.Decrypt(after);

	srv->run();
	return 0;
}
