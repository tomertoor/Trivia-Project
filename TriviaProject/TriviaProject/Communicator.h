#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <thread>
#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "OTPCryptoAlgorithm.h"

const int PORT = 42069;


class Communicator
{
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
public:
	
	void startHandleRequests();

	static Communicator* getInstance(RequestHandlerFactory& handlerFactory);

	~Communicator();
	
	Communicator(Communicator&) = delete;
	Communicator& operator=(const Communicator&) = delete;


private:

	static Communicator* instance;

	Communicator(RequestHandlerFactory& handlerFactory);

	void bindAndListen();
	void handleNewClient(SOCKET sock);

	void sendData(const SOCKET sc, const std::string message);
	std::string getPartFromSocket(const SOCKET sc, const int bytesNum);
	std::string getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags);
	std::string bufferToString(Buffer buf);

	Buffer stringToBuffer(const std::string& msg);

	void checkBroadcastToRoom(SOCKET sock);
};