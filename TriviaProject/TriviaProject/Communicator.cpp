#include "Communicator.h"

#define LOGIN "5"
#define SIGN_UP "1"

Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)

{
}

Communicator::~Communicator()
{
}

/*Base functions which utilizes all the other functions in the class to make it handle requests.
* Input - None.
* Output - None.
*/
void Communicator::startHandleRequests()
{
	this->bindAndListen();

	while (true)
	{
		SOCKET client_socket = accept(this->m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;
		// the function that handle the conversation with the client
		std::thread handleThread(&Communicator::handleNewClient, this, client_socket);//, std::ref(*this));
		handleThread.detach();

	}
}

/*Handles of binding and to start the listen to the port specified.
* Input - None.
* Output - None.
*/
void Communicator::bindAndListen()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

	struct sockaddr_in sa = { 0 };


	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << std::endl;
}

/*The connector thread for each client, Currently does what the v1.0.1 requirements says
* Input - sock: the socket to handle
* Output - None.
*/
void Communicator::handleNewClient(SOCKET sock)
{
	LoginRequestHandler* handler = new LoginRequestHandler();
	this->m_clients.insert(std::pair<SOCKET, IRequestHandler*>(sock, handler));
	/*this->sendData(sock, "Hello");
	std::string userData = this->getPartFromSocket(sock, 5);
	std::cout << "User sent: " << userData << std::endl;
	delete this->m_clients.find(sock)->second;
	this->m_clients.erase(sock);*/
	
	Requests::RequestInfo ri;
	ri.buffer = Buffer();
	ri.id = stoi(this->getPartFromSocket(sock, 1));
	for (int i = 0; i < stoi(this->getPartFromSocket(sock, 4)) ; i++)
	{
		ri.buffer.buffer.push_back(this->getPartFromSocket(sock, 1)[0]);
	}
	if (handler->isRequestRelevant(ri))
	{
		if (ri.id == 1)
		{

		}
		else(ri.id == 5)
		{

		}
	}
	else
	{

	}
}

/*Helper function for sending data
* Input - sc : the socket to receive from, message: the message to send
* Output - None.
*/
void Communicator::sendData(const SOCKET sc, const std::string message)
{
	const char* data = message.c_str();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}
/*Helper function for recieving data
* Input - sc : the socket to receive from, bytesNum: the amount of bytes to get
* Output - the message
*/
std::string Communicator::getPartFromSocket(const SOCKET sc, const int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

/*Helper function for recieving data
* Input - sc : the socket to receive from, bytesNum: the amount of bytes to get, flags: keep 0
* Output - the message
*/
std::string Communicator::getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags)
{
	if (bytesNum == 0)
	{
		return "";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);
	data[bytesNum] = '\0';
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}
	std::string received(data);
	delete[] data;
	return received;
}
