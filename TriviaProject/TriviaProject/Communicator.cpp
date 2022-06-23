#include "Communicator.h"

#define LOGIN "5"
#define SIGN_UP "1"

Communicator* Communicator::instance = nullptr;

Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory), m_serverSocket(0)

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

		std::cout << "Client at socket " <<  client_socket << " accepted .Server and client can speak" << std::endl;
		// the function that handle the conversation with the client
		std::thread handleThread(&Communicator::handleNewClient, this, client_socket);//, std::ref(*this));
		handleThread.detach();

	}
}

//get instance for singleton
Communicator* Communicator::getInstance(RequestHandlerFactory& handlerFactory)
{
    if (instance == nullptr)
    {
        instance = new Communicator(handlerFactory);
    }
    return instance;
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

    IRequestHandler* handler = new LoginRequestHandler();
    this->m_clients.insert(std::pair<SOCKET, IRequestHandler*>(sock, handler));
    Requests::RequestInfo ri;

    try
    {
        while (true)
        {
            PAZCryptoAlgorithm algorithm(this->getPartFromSocket(sock, KEY_LEN));
            ri.id = stoi(algorithm.Decrypt(stringToBuffer(this->getPartFromSocket(sock, CODE_LEN))));
            int len = stoi(algorithm.Decrypt(stringToBuffer(this->getPartFromSocket(sock, SIZE_LEN))));
            ri.buffer = stringToBuffer(algorithm.Decrypt(stringToBuffer(this->getPartFromSocket(sock, len))));

            if (handler->isRequestRelevant(ri))
            {
                Requests::RequestResult rs = handler->handleRequest(ri);
                if (rs.newHandler != nullptr)
                    handler = rs.newHandler;
                try
                {
                    this->sendData(sock, this->bufferToString(rs.response));
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
            else
            {
                Responses::ErrorResponse er;
                er.message = "Invalid code";
                Buffer b = JsonResponsePacketSerializer::serializeResponse(er);
                try
                {
                    this->sendData(sock, this->bufferToString(b));
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    }
    catch (...)
    {
        if (this->m_clients.find(sock) != this->m_clients.end())
        {
            delete (this->m_clients.find(sock)->second);
            this->m_clients.erase(sock);
        }
        std::cout << "User at socket " << sock << " disconnected." << std::endl;
    }
}

/*Helper function for sending data
* Input - sc : the socket to receive from, message: the message to send
* Output - None.
*/
void Communicator::sendData(const SOCKET sc, const std::string message)
{
    PAZCryptoAlgorithm algorithm("");
    auto buf = algorithm.Encrypt(message);
    std::string sen = algorithm.GetKey();
    for (unsigned int i = 0; i < buf.buffer.size(); i++)
        sen.push_back(buf.buffer[i]);
	const char* data = sen.c_str();

	if (send(sc, data, sen.size(), 0) == INVALID_SOCKET)
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

/*
Helper function to convert buffer to string
input: buffer
output: string with the content of the buffer
*/
std::string Communicator::bufferToString(Buffer buf)
{
    std::string data = "";
    for (unsigned int i = 0; i < buf.buffer.size(); i++)
        data += buf.buffer[i];
    return data;
}

//helper function to convert string to buffer
Buffer Communicator::stringToBuffer(const std::string& msg)
{
    Buffer buf;
    for (unsigned int i = 0; i < msg.size(); i++)
        buf.buffer.push_back(msg[i]);
    return buf;
}
