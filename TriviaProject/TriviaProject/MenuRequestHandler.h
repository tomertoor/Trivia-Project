#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RequestHandlerFactory.h"

class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	RequestHandlerFactory& m_handlerFactory;

	Requests::RequestResult signout(Requests::RequestInfo info);
	Requests::RequestResult getRooms(Requests::RequestInfo info);
	Requests::RequestResult getPlayersInRoom(Requests::RequestInfo info);
	Requests::RequestResult getPersonalStats(Requests::RequestInfo info);
	Requests::RequestResult getHighScore(Requests::RequestInfo info);
	Requests::RequestResult joinRoom(Requests::RequestInfo info);
	Requests::RequestResult createRoom(Requests::RequestInfo info);

public:
	MenuRequestHandler();
	~MenuRequestHandler();

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
};